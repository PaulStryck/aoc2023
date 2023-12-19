#ifndef PARSER_HEADER
#define PARSER_HEADER

#include "grammar/Absyn.H"
#include <iostream>
#include <map>
#include <functional>

namespace prob_1 {
/*
 * KCell (Hyperrectangle)
 */
template<uint8_t K>
struct KCell {
  std::array<std::pair<int, int>, K> intervals;
};

/*
 * Translate part categories to be used as array indices
 */
enum PartCat {X=0, M, A, S};
using Part = std::array<int, 4>;

using Label = std::string;
using State = std::pair<Label, uint8_t>;

enum Compare {LT, GT};

class Rule {
public:
  virtual ~Rule(){}

  /*
   * Evaluating a part will return the new State
   */
  virtual State eval(Part p) = 0;

  /*
   * Evaluating a Rule on a 4-Cell will split the 4-Cell into
   * possibly multiple new 4-Cells where the entire 4-Cell
   * will transition to the new state.
   */
  virtual std::vector<std::pair<KCell<4>, State>> eval(KCell<4> c) = 0;
};

/*
 * The RuleFallback always transitions to a fixed State, regardless of input
 */
class RuleFallback: public Rule {
private:
  State next;

public:
  RuleFallback(State next) : next{next}{}
  ~RuleFallback(){}

  State eval(Part p){ return next; }

  [[nodiscard]] std::vector<std::pair<KCell<4>, State>> eval(KCell<4> c){
    return {{c, next}};
  }
};

/*
 * RuleEval will transition to different states depending on a comparision result
 */
class RuleEval: public Rule {
private:
  PartCat lhs;
  Compare cmp;
  int rhs;

  std::function<bool (int, int)> op;

  State nextT;
  State nextF;

public:
  RuleEval(PartCat lhs, Compare cmp, int rhs, State nextT, State nextF) : lhs{lhs}, cmp{cmp}, rhs{rhs}, nextT{nextT}, nextF{nextF} {
    if(cmp == Compare::LT)
      op = std::less<int>{};
    else if(cmp == Compare::GT)
      op = std::greater<int>{};
    else
      throw;
  };
  ~RuleEval(){}

  State eval(Part p) {
      if(op(p[lhs], rhs))
        return nextT;
      return nextF;
  }

  /*
   * Split KCell<4> c along rhs = lhs Hyperplane
   */
  [[nodiscard]] std::vector<std::pair<KCell<4>, State>> eval(KCell<4> c){
    std::vector<std::pair<KCell<4>, State>> res{};

    // [0, 5] : (6, GT) -> {[0,  5]}
    // [0, 5] : (6, LT) -> {[0,  5]}
    //
    // [1, 5] : (0, GT) -> {[1,  5]}
    // [1, 5] : (0, LT) -> {[1,  5]}
    if(rhs < c.intervals[lhs].first || rhs > c.intervals[lhs].second)
      return {{c, op(c.intervals[lhs].first, rhs) ? nextT : nextF}};

    // [0, 5] : (5, GT) -> {[0,  5]}
    // [0, 5] : (5, LT) -> {[0,  4], [5,  5]}
    //
    // [0, 5] : (0, GT) -> {[0,  0], [1,  5]}
    // [0, 5] : (0, LT) -> {[0,  5]}
    //
    if(rhs == c.intervals[lhs].first || rhs == c.intervals[lhs].second){
      // not needed atm
      throw;
    }

    // [0,10] : (5, GT) -> {[0,  5]: F, [6, 10]: T}
    // [0,10] : (5, LT) -> {[5, 10]: F, [0,  4]: T}
    KCell<4> a, b;
    a = std::move(c);
    b.intervals = a.intervals;  // invoke array copy
    if (cmp == Compare::GT) {
      a.intervals[lhs].first = std::max(a.intervals[lhs].first, rhs + 1);
      b.intervals[lhs].second = std::min(b.intervals[lhs].second, rhs);
    } else if (cmp == Compare::LT) {
      a.intervals[lhs].second = std::min(a.intervals[lhs].second, rhs-1);
      b.intervals[lhs].first = std::max(b.intervals[lhs].first, rhs);
    }

    return {{a, nextT}, {b, nextF}};
  }
};

using States = std::map<State, std::shared_ptr<Rule>>;


class Validator : public Visitor
{
 public:
  Validator(void);
  ~Validator(void);
  std::pair<States, std::vector<Part>> validate(Visitable *v);

  void visitRecords(Records *p);
  void visitWorkflows(Workflows *p);
  void visitWorkflow(Workflow *p);
  void visitInstruction(Instruction *p);
  void visitCat(Cat *p);
  void visitComp(Comp *p);
  void visitData(Data *p);
  void visitRatings(Ratings *p);
  void visitRating(Rating *p);
  void visitPRecords(PRecords *p);
  void visitPWorkflows(PWorkflows *p);
  void visitEWorkflow(EWorkflow *p);
  void visitEInstructionExp(EInstructionExp *p);
  void visitEInstructionJmp(EInstructionJmp *p);
  void visitCCompLT(CCompLT *p);
  void visitCCompGT(CCompGT *p);
  void visitCatX(CatX *p);
  void visitCatM(CatM *p);
  void visitCatA(CatA *p);
  void visitCatS(CatS *p);
  void visitPData(PData *p);
  void visitERatings(ERatings *p);
  void visitERating(ERating *p);
  void visitListWorkflow(ListWorkflow *p);
  void visitListInstruction(ListInstruction *p);
  void visitListRatings(ListRatings *p);
  void visitListRating(ListRating *p);


  void visitInteger(Integer x);
  void visitChar(Char x);
  void visitDouble(Double x);
  void visitString(String x);
  void visitIdent(Ident x);
  void visitUName(UName x);

 protected:
  States states;
  std::vector<Part> parts;

  State currState;
  PartCat currCat;
  Compare currComp;

  Part currPart;
};
}
#endif

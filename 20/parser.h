#ifndef PARSER_HEADER
#define PARSER_HEADER

#include "grammar/Absyn.H"
#include <iostream>
#include <map>
#include <functional>
#include <algorithm>

namespace prob_1 {

enum PulseT {LOW, HIGH};
enum GateT {FF, Conj, Broadcaster};

struct PulseIn {
  std::string src;
  PulseT p;
};

struct Pulse {
  std::string src;
  std::string dest;
  PulseT p;
};


class Gate {
  public:
    std::string name;

  protected:
    std::vector<std::string> outputs;

  public:
    Gate(std::string name) : name{name}, outputs{} {}
    virtual ~Gate(){}
    virtual std::vector<Pulse> recv(PulseIn p) = 0;

    void addOutput(std::string o){
      this->outputs.push_back(o);
    }
};


class GateFF : public Gate {
  bool state;

  public:
  GateFF(std::string name) : Gate{name}, state{false} {}
  ~GateFF(){}

  std::vector<Pulse>recv(PulseIn p){
    std::vector<Pulse> out {};

    if(p.p == PulseT::LOW){
      state = !state;
      PulseT outPulse = state ? PulseT::HIGH : PulseT::LOW;
      for(auto dest: outputs){
        out.emplace_back(Pulse{this->name, dest, outPulse});
      }
    }

    return out;
  }
};

class GateConj : public Gate {
  protected:
    std::unordered_map<std::string, PulseT> inputs;

  public:
  GateConj(std::string name) : Gate{name}, inputs{} {}
  ~GateConj(){}

  std::vector<Pulse>recv(PulseIn p){
    PulseT outP{PulseT::HIGH};
    std::vector<Pulse> out {};
    inputs[p.src] = p.p;

    bool foo = std::all_of(inputs.begin(), inputs.end(), [](auto a){return a.second == PulseT::HIGH;});
    if(foo){
      outP = PulseT::LOW;
    }

    for(auto dest: outputs)
      out.emplace_back(Pulse{this->name, dest, outP});

    return out;
  }

  void addInput(std::string i){
    inputs[i] = PulseT::LOW;
  }
};

class GateBroadcast : public Gate {
  public:
  GateBroadcast(std::string name) : Gate{name} {}
  ~GateBroadcast(){}

  std::vector<Pulse>recv(PulseIn p){
    std::vector<Pulse> out{};
      for(auto dest: outputs)
        out.emplace_back(Pulse({this->name, dest, p.p}));

    return out;
  }
};

using Circuit = std::unordered_map<std::string, std::shared_ptr<Gate>>;

class Validator : public Visitor
{
 public:
  Validator(void);
  ~Validator(void);
  [[nodiscard]] Circuit validate(Visitable *v);

  void visitRecords(Records *p);
  void visitModule(Module *p);
  void visitCat(Cat *p);
  void visitPRecords(PRecords *p);
  void visitEModule(EModule *p);
  void visitEBroadcaster(EBroadcaster *p);
  void visitCatFF(CatFF *p);
  void visitCatC(CatC *p);
  void visitListModule(ListModule *p);
  void visitListUName(ListUName *p);


  void visitInteger(Integer x);
  void visitChar(Char x);
  void visitDouble(Double x);
  void visitString(String x);
  void visitIdent(Ident x);
  void visitUName(UName x);

 protected:
  std::shared_ptr<Gate> g;
  std::string currName;
  Circuit c;
  std::unordered_map<std::string, std::vector<std::string>> insOuts;
};
}
#endif

#ifndef VALIDATOR_P1_HEADER
#define VALIDATOR_P1_HEADER

#include "grammar/Absyn.H"
namespace prob_1 {
class Validator : public Visitor
{
 public:
  Validator(void);
  ~Validator(void);
  int validate(Visitable *v);

  void visitRecords(Records *p); /* abstract class */
  void visitPRecords(PRecords *p);
  void visitRecord(Record *p); /* abstract class */
  void visitERecord(ERecord *p);
  void visitListRecord(ListRecord *p);
  void visitListSubset(ListSubset *p);
  void iterListSubset(ListSubset::const_iterator i, ListSubset::const_iterator j);
  void visitSubset(Subset *p); /* abstract class */
  void visitLSubset(LSubset *p);
  void visitListDraw(ListDraw *p);
  void iterListDraw(ListDraw::const_iterator i, ListDraw::const_iterator j);
  void visitDraw(Draw *p); /* abstract class */
  void visitLDraw(LDraw *p);
  void visitColor(Color *p); /* abstract class */
  void visitCColorRed(CColorRed *p);
  void visitCColorGreen(CColorGreen *p);
  void visitCColorBlue(CColorBlue *p);

  void visitInteger(Integer x) {};
  void visitChar(Char x){};
  void visitDouble(Double x){};
  void visitString(String x){};
  void visitIdent(Ident x){};
 protected:
  int currDrawCount = 0;
  int idCount = 0;
  bool possible = true;
};
}
#endif

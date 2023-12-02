#include "prob_1.h"

namespace prob_1 {
Validator::Validator() : currDrawCount(0), idCount(0), possible(true) {}

Validator::~Validator(){}


int Validator::validate(Visitable *v){
  v->accept(this);
  return idCount;
}

void Validator::visitRecords(Records *p){} /* abstract class */
void Validator::visitPRecords(PRecords *p){
  if (p->listrecord_)  p->listrecord_->accept(this);
}

void Validator::visitRecord(Record *p){} /* abstract class */
void Validator::visitERecord(ERecord *p){
  if (p->listsubset_)  p->listsubset_->accept(this);

  if(possible)
    idCount += p->integer_;
}

void Validator::visitListRecord(ListRecord *listrecord){
  for (ListRecord::const_iterator i = listrecord->begin() ; i != listrecord->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void Validator::visitListSubset(ListSubset *listsubset){
  possible = true;

  for (ListSubset::const_iterator i = listsubset->begin() ; i != listsubset->end() ; ++i)
  {
    if(possible)
      (*i)->accept(this);
    else
      break;
  }
}

void Validator::visitSubset(Subset *p){} /* abstract class */
void Validator::visitLSubset(LSubset *p){
  if (p->listdraw_)  p->listdraw_->accept(this);
}

void Validator::visitListDraw(ListDraw *listdraw){
  for (ListDraw::const_iterator i = listdraw->begin() ; i != listdraw->end() ; ++i)
  {
    (*i)->accept(this);
  }
}
void Validator::visitDraw(Draw *p){} /* abstract class */
void Validator::visitLDraw(LDraw *p){
  currDrawCount = p->integer_;
  if (p->color_)  p->color_->accept(this);
}

void Validator::visitColor(Color *p){} /* abstract class */
void Validator::visitCColorRed(CColorRed *p){
  if(currDrawCount > 12) possible = false;
}
void Validator::visitCColorGreen(CColorGreen *p){
  if(currDrawCount > 13) possible = false;
}
void Validator::visitCColorBlue(CColorBlue *p){
  if(currDrawCount > 14) possible = false;
}
};

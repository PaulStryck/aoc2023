#include "prob_2.h"
#include <iostream>

namespace prob_2 {
Validator::Validator() : power(0), minColors{0,0,0}, currDrawCount(0){}

Validator::~Validator(){}


int Validator::validate(Visitable *v){
  v->accept(this);
  return power;
}

void Validator::visitRecords(Records *p){} /* abstract class */
void Validator::visitPRecords(PRecords *p){
  if (p->listrecord_)  p->listrecord_->accept(this);
}

void Validator::visitRecord(Record *p){} /* abstract class */
void Validator::visitERecord(ERecord *p){
  minColors[RED]   = 0;
  minColors[GREEN] = 0;
  minColors[BLUE]  = 0;

  if (p->listsubset_)  p->listsubset_->accept(this);

  power += minColors[RED] * minColors[GREEN] * minColors[BLUE];
}

void Validator::visitListRecord(ListRecord *listrecord){
  for (ListRecord::const_iterator i = listrecord->begin() ; i != listrecord->end() ; ++i)
  {
    (*i)->accept(this);
  }
}

void Validator::visitListSubset(ListSubset *listsubset){
  for (ListSubset::const_iterator i = listsubset->begin() ; i != listsubset->end() ; ++i)
  {
    (*i)->accept(this);
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
  if(currDrawCount > minColors[RED])
    minColors[RED] = currDrawCount;
}
void Validator::visitCColorGreen(CColorGreen *p){
  if(currDrawCount > minColors[GREEN])
    minColors[GREEN] = currDrawCount;
}
void Validator::visitCColorBlue(CColorBlue *p){
  if(currDrawCount > minColors[BLUE])
    minColors[BLUE] = currDrawCount;
}
};

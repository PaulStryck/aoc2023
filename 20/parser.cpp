#include "parser.h"

#include <memory>
#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>

namespace prob_1 {
Validator::Validator(){};
Validator::~Validator(void){};

[[nodiscard]] Circuit Validator::validate(Visitable *v){
  v->accept(this);

  for(auto it = this->insOuts.cbegin(); it != this->insOuts.cend(); ++it){
    std::shared_ptr<Gate> ap = this->c[it->first];
    if (GateConj* bp = dynamic_cast<GateConj*>(ap.get())){
      for(auto foo: it->second)
        bp->addInput(foo);
    }
  }
  return c;
}

void Validator::visitRecords(Records *p){}
void Validator::visitModule(Module *p){}
void Validator::visitCat(Cat *p){}

void Validator::visitPRecords(PRecords *p){
  if(p->listmodule_)
    p->listmodule_->accept(this);
}

void Validator::visitEModule(EModule *p){
  this->currName = p->uname_;
  p->cat_->accept(this);
  p->listuname_->accept(this);
  (this->c)[this->currName] = this->g;
}

void Validator::visitEBroadcaster(EBroadcaster *p){
  this->currName = p->uname_;
  GateBroadcast _g{this->currName};
  this->g = std::make_shared<GateBroadcast>(_g);
  p->listuname_->accept(this);
  (this->c)[this->currName] = this->g;
}

void Validator::visitCatFF(CatFF *p){
  GateFF _g{this->currName};
  this->g = std::make_shared<GateFF>(_g);
}

void Validator::visitCatC(CatC *p){
  GateConj _g{this->currName};
  this->g = std::make_shared<GateConj>(_g);
}

void Validator::visitListModule(ListModule *p){
  for(ListModule::const_iterator it = p->begin(); it != p->end(); ++it)
    (*it)->accept(this);
}

void Validator::visitListUName(ListUName *p){
  for(ListUName::const_iterator it = p->begin(); it != p->end(); ++it){
    this->g->addOutput(*it);
    this->insOuts[*it].emplace_back(this->g->name);
  }
}


void Validator::visitInteger(Integer x){}
void Validator::visitChar(Char x){}
void Validator::visitDouble(Double x){}
void Validator::visitString(String x){}
void Validator::visitIdent(Ident x){}
void Validator::visitUName(UName x){}
}

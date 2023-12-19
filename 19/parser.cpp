#include "parser.h"
#include <memory>
#include <iostream>
#include <vector>
#include <functional>

namespace prob_1 {
Validator::Validator() : states{}, parts{}, currState{}, currCat{}, currComp{}, currPart{} {}

Validator::~Validator(){}

[[nodiscard]] std::pair<States, std::vector<Part>> Validator::validate(Visitable *v){
  v->accept(this);
  return std::make_pair(this->states, this->parts);
}

void Validator::visitPRecords(PRecords *p){
  // Prodcue States
  if (p->workflows_)  p->workflows_->accept(this);

  // Produce Parts
  if (p->data_) p->data_->accept(this);
};

void Validator::visitPWorkflows(PWorkflows *p){
  if (p->listworkflow_)  p->listworkflow_->accept(this);
};

void Validator::visitEWorkflow(EWorkflow *p){
  // Start parsing new state with laben p->uname_ at default pos 0
  this->currState = {p->uname_, 0};
  if(p->listinstruction_) p->listinstruction_->accept(this);
};

void Validator::visitEInstructionExp(EInstructionExp *p){
  // Produce Categorey (one of: X, M, A, S)
  p->cat_->accept(this);

  // Produce the comparision mode (>, <)
  p->comp_->accept(this);

  // Add finished rule to state map
  RuleEval r{this->currCat, this->currComp, p->integer_, {p->uname_, 0}, {this->currState.first, this->currState.second+1}};
  this->states[this->currState] = std::make_shared<RuleEval>(r);

  // Next state to parse will have its counter incremented
  ++this->currState.second;
};


void Validator::visitEInstructionJmp(EInstructionJmp *p){
  UName l = p->uname_;

  // Add the Fallback rule to state map
  RuleFallback r{{l, 0}};
  this->states[this->currState] = std::make_shared<RuleFallback>(r);
};

void Validator::visitCCompLT(CCompLT *p){
  this->currComp = Compare::LT;
};

void Validator::visitCCompGT(CCompGT *p){
  this->currComp = Compare::GT;
};

void Validator::visitCatX(CatX *p){
  this->currCat = PartCat::X;
};

void Validator::visitCatM(CatM *p){
  this->currCat = PartCat::M;
};

void Validator::visitCatA(CatA *p){
  this->currCat = PartCat::A;
};

void Validator::visitCatS(CatS *p){
  this->currCat = PartCat::S;
};

void Validator::visitPData(PData *p){
  if (p->listratings_)  p->listratings_->accept(this);
};

void Validator::visitERatings(ERatings *p){
  if(p->listrating_) p->listrating_->accept(this);
  this->parts.push_back(this->currPart);
};

void Validator::visitERating(ERating *p){
  p->cat_->accept(this);
  currPart[currCat] = p->integer_;
};

void Validator::visitListWorkflow(ListWorkflow *p){
  for(ListWorkflow::const_iterator it = p->begin(); it != p->end(); ++it)
    (*it)->accept(this);
};

void Validator::visitListInstruction(ListInstruction *p){
  for (ListInstruction::const_iterator i = p->begin() ; i != p->end() ; ++i)
    (*i)->accept(this);
};

void Validator::visitListRatings(ListRatings *p){
  for(ListRatings::const_iterator it = p->begin(); it != p->end(); ++it)
    (*it)->accept(this);
};

void Validator::visitListRating(ListRating *p){
  for(ListRating::const_iterator it = p->begin(); it != p->end(); ++it)
    (*it)->accept(this);
};


void Validator::visitInteger(Integer x){};
void Validator::visitChar(Char x){};
void Validator::visitDouble(Double x){};
void Validator::visitString(String x){};
void Validator::visitIdent(Ident x){};
void Validator::visitUName(UName x){};

void Validator::visitRecords(Records *p){};

void Validator::visitWorkflows(Workflows *p){};
void Validator::visitWorkflow(Workflow *p){};
void Validator::visitInstruction(Instruction *p){};
void Validator::visitCat(Cat *p){};
void Validator::visitComp(Comp *p){};
void Validator::visitData(Data *p){};
void Validator::visitRatings(Ratings *p){};
void Validator::visitRating(Rating *p){};
}

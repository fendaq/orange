#include "examplegen.hpp"
#include "contingency.hpp"

#include "tdidt_stop.ppp"


bool TTreeStopCriteria::operator()(PExampleGenerator gen, const int &, PDomainContingency ocont)
{ int nor = gen->numberOfExamples();
  if ((nor==0) || (nor==1))
    return true;      // example set is too small

  char vt = gen->domain->classVar->varType;
  if (vt!=TValue::INTVAR)
    return false;  // class is continuous, may continue

  // is there more than one class left?
  if (ocont) {
    char ndcf = 0;
    TDiscDistribution const &dva=CAST_TO_DISCDISTRIBUTION(ocont->classes);
    const_ITERATE(TDiscDistribution, ci, dva)
      if ((*ci>0) && (++ndcf==2))
        return false; // at least two classes, may continue
  }
  
  else {  
    TExampleIterator ei = gen->begin();
    TValue fv = (*ei).getClass();

    while(fv.isSpecial() && ++ei)
      fv = (*ei).getClass();
    if (!ei)
      return true;

    const int fvi = fv.intV;

    while(++ei) {
      TValue &cval = (*ei).getClass();
      if (!cval.isSpecial() && (cval.intV != fvi))
        return false; // yes, may continue
    }
  }

  return true; // no, there's just one class left!
}


TTreeStopCriteria_common::TTreeStopCriteria_common(float aMaxMajor, float aMinExamples)
: maxMajority(aMaxMajor),
  minInstances(aMinExamples)
{}


TTreeStopCriteria_common::TTreeStopCriteria_common(const TTreeStopCriteria_common &old)
: TTreeStopCriteria(old),
  maxMajority(old.maxMajority),
  minInstances(old.minInstances)
{}


bool TTreeStopCriteria_common::operator()(PExampleGenerator gen, const int &weight, PDomainContingency ocont)
{ if (TTreeStopCriteria::operator()(gen, weight, ocont)) 
    return true; // inherited method says its enough

  PDistribution classDist = ocont ? ocont->classes : getClassDistribution(gen, weight);
  if (classDist->abs<minInstances)
    return true; // not enough examples

  float limit = maxMajority*classDist->abs;
  TDiscDistribution *ddva = classDist.AS(TDiscDistribution);
  if (ddva) {
    const_PITERATE(TDiscDistribution, ci, ddva)
      if (*ci>limit)
        return true;
  }
  else {
    TContDistribution *cdva = classDist.AS(TContDistribution);
    const_PITERATE(TContDistribution, ci, cdva)
      if ((*ci).second>limit)
        return true;
  }

  return false;
}


#include "boolcnt.hpp"
#include "root.hpp"
#include "vars.hpp"

WRAPPER(SubsetsGenerator_iterator)


class ORANGE_API TSubsetsGenerator : public TOrange {
public:
  __REGISTER_ABSTRACT_CLASS

  PVarList varList; //P a set of attributes from which subsets are generated

  TSubsetsGenerator();
  TSubsetsGenerator(PVarList);
  virtual PSubsetsGenerator_iterator operator()() = 0;
};


class ORANGE_API TSubsetsGenerator_iterator : public TOrange {
public:
  __REGISTER_ABSTRACT_CLASS

  PVarList varList; //P a set of attributes from which subsets are generated

  TSubsetsGenerator_iterator(PVarList);
  virtual bool operator()(TVarList &) = 0;
};


WRAPPER(SubsetsGenerator);


class ORANGE_API TSubsetsGenerator_constSize : public TSubsetsGenerator {
public:
  __REGISTER_CLASS

  int B; //P subset size

  TSubsetsGenerator_constSize(int B);
  TSubsetsGenerator_constSize(PVarList vl, int B);
  virtual PSubsetsGenerator_iterator operator()();
};


class ORANGE_API TSubsetsGenerator_constSize_iterator : public TSubsetsGenerator_iterator {
public:
  __REGISTER_CLASS

  bool moreToCome; //PR
  TCounter counter;

  TSubsetsGenerator_constSize_iterator(PVarList, int aB);
  virtual bool operator()(TVarList &);
};



class ORANGE_API TSubsetsGenerator_minMaxSize : public TSubsetsGenerator {
public:
  __REGISTER_CLASS

  int min; //P minimal subset size
  int max; //P maximal subset size

  TSubsetsGenerator_minMaxSize(int amin, int amax); // can be -1 to ignore
  TSubsetsGenerator_minMaxSize(PVarList, int amin, int amax); // can be -1 to ignore
  virtual PSubsetsGenerator_iterator operator()();
};


class ORANGE_API TSubsetsGenerator_minMaxSize_iterator : public TSubsetsGenerator_iterator {
public:
  __REGISTER_CLASS

  int B; //PR
  int max; //P maximal subset size

  bool moreToCome; //PR
  TCounter counter;

  TSubsetsGenerator_minMaxSize_iterator(PVarList, int amin, int amax); // can be -1 to ignore
  virtual bool operator()(TVarList &);
};



class ORANGE_API TSubsetsGenerator_constant : public TSubsetsGenerator {
public:
  __REGISTER_CLASS

  PVarList constant; //P a subset that is returned (once!)

  TSubsetsGenerator_constant();
  TSubsetsGenerator_constant(PVarList vl, PVarList cons);

  virtual PSubsetsGenerator_iterator operator()();
};


class ORANGE_API TSubsetsGenerator_constant_iterator : public TSubsetsGenerator_iterator {
public:
  __REGISTER_CLASS

  PVarList constant; //P a subset that is returned (once!)
  bool moreToCome; //P

  TSubsetsGenerator_constant_iterator();
  TSubsetsGenerator_constant_iterator(PVarList vl, PVarList cons);

  virtual bool operator()(TVarList &);
};


class ORANGE_API TSubsetsGenerator_withRestrictions : public TSubsetsGenerator {
public:
  __REGISTER_CLASS

  PSubsetsGenerator subGenerator; //P subset generator
  PVarList required; //P set of required attributes
  PVarList forbidden; //P set of forbidden attributes
  PVarListList forbiddenSubSubsets; //P set of forbidden subsets (attributes that must not occur together)

  TSubsetsGenerator_withRestrictions(PSubsetsGenerator sub=PSubsetsGenerator());
  TSubsetsGenerator_withRestrictions(PSubsetsGenerator sub, const TVarList &areq, const TVarList &aforb);

  virtual PSubsetsGenerator_iterator operator()();
};


class ORANGE_API TSubsetsGenerator_withRestrictions_iterator : public TSubsetsGenerator_iterator {
public:
  __REGISTER_CLASS

  PSubsetsGenerator_iterator subGenerator_iterator; //P subset generator
  PVarList required; //P set of required attributes
  PVarList forbidden; //P set of forbidden attributes
  PVarListList forbiddenSubSubsets; //P set of forbidden subsets (attributes that must not occur together)

  TSubsetsGenerator_withRestrictions_iterator();
  TSubsetsGenerator_withRestrictions_iterator(PSubsetsGenerator_iterator sub, PVarList areq, PVarList aforb);

  virtual bool operator()(TVarList &);
};

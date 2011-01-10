// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    ub_str_seq.h
//
// = DESCRIPTION
//     Tests sequences of strings
//
// = AUTHORS
//      Aniruddha Gokhale
//
// ============================================================================

#ifndef PARAM_TEST_UNBOUNDED_STRING_SEQUENCE_H
#define PARAM_TEST_UNBOUNDED_STRING_SEQUENCE_H

#include "param_testC.h"

// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
//           test typedefed sequences (in our case, sequences of strings)
// =*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*

class Test_String_Sequence
{
public:
  Test_String_Sequence (void);
  // ctor

  ~Test_String_Sequence (void);
  // dtor

  int run_sii_test (Param_Test_ptr objref);
  // run the SII test

  const char *opname (void) const;
  // return operation name

  int init_parameters (Param_Test_ptr objref);
  // set values for parameters

  int reset_parameters (void);
  // reset values for CORBA

  CORBA::Boolean check_validity (void);
  // check if results are valid

  CORBA::Boolean check_validity (CORBA::Request_ptr req);
  // check if results are valid. This is used for DII results

  void print_values (void);
  // print all the values

  void dii_req_invoke (CORBA::Request *);
  // invoke DII request with appropriate exception handling.

private:
  char *opname_;
  // operation name

  CORBA::StringSeq_var in_;
  // in parameter

  CORBA::StringSeq_var inout_;
  // inout parameter

  CORBA::StringSeq_var out_;
  // out parameter

  CORBA::StringSeq_var ret_;
  // return value
};

#endif /* PARAM_TEST_UNBOUNDED_STRING_SEQUENCE_H */
//============================================================================
//
//   SSSS    tt          lll  lll       
//  SS  SS   tt           ll   ll        
//  SS     tttttt  eeee   ll   ll   aaaa 
//   SSSS    tt   ee  ee  ll   ll      aa
//      SS   tt   eeeeee  ll   ll   aaaaa  --  "An Atari 2600 VCS Emulator"
//  SS  SS   tt   ee      ll   ll  aa  aa
//   SSSS     ttt  eeeee llll llll  aaaaa
//
// Copyright (c) 1995-2005 by Bradford W. Mott and the Stella team
//
// See the file "license" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//
// $Id: MultExpression.hxx,v 1.2 2005/07/27 01:36:51 urchlay Exp $
//============================================================================

#ifndef MULT_EXPRESSION_HXX
#define MULT_EXPRESSION_HXX

#include "bspf.hxx"
#include "Expression.hxx"

/**
  @author  B. Watson
  @version $Id: MultExpression.hxx,v 1.2 2005/07/27 01:36:51 urchlay Exp $
*/
class MultExpression : public Expression
{
  public:
    MultExpression(Expression *left, Expression *right);
    uInt16 evaluate() { return myLHS->evaluate() * myRHS->evaluate(); }
};

#endif


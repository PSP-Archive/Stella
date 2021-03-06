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
// $Id: ModExpression.hxx,v 1.3 2005/09/15 19:43:36 stephena Exp $
//============================================================================

#ifndef MOD_EXPRESSION_HXX
#define MOD_EXPRESSION_HXX

#include "bspf.hxx"
#include "Expression.hxx"

/**
  @author  B. Watson
  @version $Id: ModExpression.hxx,v 1.3 2005/09/15 19:43:36 stephena Exp $
*/
class ModExpression : public Expression
{
  public:
    ModExpression(Expression *left, Expression *right);
    uInt16 evaluate() { int rhs = myRHS->evaluate();
                        return rhs == 0 ? 0 : myLHS->evaluate() % rhs;
                      }
};

#endif

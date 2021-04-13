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
// $Id: YaccParser.hxx,v 1.6 2005/07/15 03:47:26 urchlay Exp $
//
//   Based on code from ScummVM - Scumm Interpreter
//   Copyright (C) 2002-2004 The ScummVM project
//============================================================================

#ifndef PARSER_HXX
#define PARSER_HXX

#include "Expression.hxx"

//#ifdef __cplusplus
//extern "C" {
//#endif

namespace YaccParser {
	int parse(const char *);
	Expression *getResult();
	const string& errorMessage();
};

//#ifdef __cplusplus
//}
//#endif

#endif

//
// "$Id$"
//
// NumericInput modified to only allow floating point to by
// typed. Currently this is implemented by the base class by checking
// type() but this may change in the future.
//
// Copyright 1998-2006 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems on the following page:
//
//    http://www.fltk.org/str.php
//

#ifndef fltk_FloatInput_h
#define fltk_FloatInput_h

#include "NumericInput.h"

namespace fltk {

class FL_API FloatInput : public NumericInput {
  virtual bool replace(int, int, const char*, int);
public:
  enum { FLOAT = 1, INT = 2 };
  FloatInput(int x,int y,int w,int h,const char *l = 0)
    : NumericInput(x,y,w,h,l) { type(FLOAT); }
  long lvalue() const;
  int ivalue() const { return int(lvalue()); }
  double fvalue() const;
};

}
#endif

//
// End of "$Id$".
//

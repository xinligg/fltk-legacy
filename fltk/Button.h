//
// "$Id$"
//
// Push button widget
//
// Copyright 2002 by Bill Spitzak and others.
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
// Please report all bugs and problems to "fltk-bugs@fltk.org".
//

#ifndef fltk_Button_h
#define fltk_Button_h

#ifndef fltk_Widget_h
#include "Widget.h"
#endif

namespace fltk {

class FL_API Button : public Widget {
public:
  enum { // values for type()
    NORMAL = 0,
    TOGGLE = RESERVED_TYPE+1,
    RADIO  = RESERVED_TYPE+2,
    HIDDEN = 3
  };
  bool  value() const {return Widget::value();}
  bool	value(bool);
  bool	set();
  bool	clear();
  void	setonly();
  int handle(int);
  int handle(int event, const Rectangle&);
  Button(int,int,int,int,const char * = 0);
  static NamedStyle* default_style;

  virtual void draw();
  void draw(int glyph, int glyph_width) const;
};

}

#endif

//
// End of "$Id$".
//
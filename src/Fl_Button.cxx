//
// "$Id: Fl_Button.cxx,v 1.66 2003/11/04 08:10:58 spitzak Exp $"
//
// Button widget for the Fast Light Tool Kit (FLTK).
//
// Copyright 1998-2003 by Bill Spitzak and others.
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

#include <fltk/events.h>
#include <fltk/damage.h>
#include <fltk/Button.h>
#include <fltk/Group.h>
#include <fltk/Box.h>
using namespace fltk;

bool Button::set() {
  clear_changed();
  if (!value()) {set_value(); redraw(); return true;}
  return false;
}

bool Button::clear() {
  clear_changed();
  if (value()) {clear_value(); redraw(); return true;}
  return false;
}

bool Button::value(bool v) {
  return v ? set() : clear();
}

void Button::setonly() { // set this radio button on, turn others off
  set();
  for (int i = parent()->children(); i--;) {
    Widget* o = parent()->child(i);
    if (o != this && o->type() == RADIO)
      ((Button*)o)->clear();
  }
}

static Button* held_down;

int Button::handle(int event) {
  static bool oldval;
  bool newval;
  switch (event) {
  case ENTER:
  case LEAVE:
    redraw_highlight();
  case MOVE:
    return 1;
  case PUSH:
    oldval = value();
  case DRAG:
    if (event_inside(0,0,w(),h())) {
      held_down = this;
      if (type() == RADIO) newval = 1;
      else newval = !oldval;
    } else {
      held_down = 0;
      newval = oldval;
    }
    if (value(newval) && when()&WHEN_CHANGED) do_callback();
    return 1;
  case RELEASE:
    redraw(DAMAGE_VALUE);
    held_down = 0;
    if (value() == oldval) return 1;
    if (type() == RADIO)
      setonly();
    else if (type()) // TOGGLE
      ; // leave it as set
    else {
      value(oldval);
      if (when() & WHEN_CHANGED) do_callback();
    }
    if (when() & WHEN_RELEASE) do_callback(); else set_changed();
    return 1;
  case FOCUS:
    redraw(1); // minimal redraw to just add the focus box
    // grab initial focus if we are an ReturnButton:
    return shortcut()=='\r' ? 2 : 1;
  case UNFOCUS:
    redraw(DAMAGE_HIGHLIGHT);
    return 1;
  case KEY:
    if (event_key() == ' ' || event_key() == ReturnKey
	|| event_key() == KeypadEnter) goto EXECUTE;
    return 0;
  case SHORTCUT:
    if (!test_shortcut()) return 0;
  EXECUTE:
    if (type() == RADIO && !value()) {
      setonly();
      if (when() & WHEN_CHANGED) do_callback();
    } else if (type()) { // TOGGLE
      value(!value());
      if (when() & WHEN_CHANGED) do_callback();
    }
    if (when() & WHEN_RELEASE) do_callback(); else set_changed();
    return 1;
  default:
    return 0;
  }
}

////////////////////////////////////////////////////////////////

#include <fltk/draw.h>

extern Widget* fl_did_clipping;

// Draw button-like widgets with an optional glyph. The glyph is given
// a size (negative to put it on the right)
void Button::draw(int glyph, int glyph_width) const
{
  // For back-compatability, setting color() or box() directly on a plain
  // button will cause it to act like buttoncolor() or buttonbox() are
  // set:
  Style localstyle;
  const Style* style = this->style();
  if (!glyph_width) {
    localstyle = *style;
    if (localstyle.color_) localstyle.buttoncolor_ = localstyle.color_;
    if (localstyle.box_) localstyle.buttonbox_ = localstyle.box_;
    if (localstyle.labelcolor_) localstyle.textcolor_ = localstyle.labelcolor_;
    style = &localstyle;
  }

  Flags flags = current_flags_highlight();

  Flags glyph_flags = flags & ~(SELECTED|HIGHLIGHT); // flags to pass to draw_glyph
  if (value()) {
    glyph_flags |= VALUE;
    // Use the pushed-in color if the user has explicitly set it
    // on this widget:
    if (style->selection_color_) glyph_flags |= SELECTED;
  }
  if (glyph_width) {
    if (this == held_down) flags |= VALUE; // outer box shows pushed-down state
  } else if (value()) {
    flags |= glyph_flags;
  }

  // only draw "inside" labels:
  bool draw_label = true;
  int x = 0, y = 0, w = this->w(), h = this->h();

  Box* box = style->buttonbox();

  if (box == NO_BOX) {
    Color bg;
    if (flags & HIGHLIGHT && (bg = style->highlight_color())) {
      setcolor(bg);
      fillrect(0, 0, w, h);
    } else if ((damage()&(DAMAGE_EXPOSE|DAMAGE_HIGHLIGHT))) {
      // erase the background so we can redraw the label in the new color:
      draw_background();
    } else if (!label()) {
      // Assumme this is a button with an animated image label.
      // we must redraw the image so it changes depending on state.
      // Partially-transparent pixels will not draw right!
    } else {
      // Don't draw the label unnecessarily:
      draw_label = false;
    }
    // this allows these buttons to be put into browser/menus:
    //fg = fl_item_labelcolor(this);
  } else {
    if ((damage()&DAMAGE_EXPOSE) && !box->fills_rectangle()) {
      // Erase the area behind non-square boxes
      draw_background();
    }
    // Draw the box:
    box->draw(0, 0, w, h, style, flags);
    box->inset(x,y,w,h);
  }

  int lx = x; int lw = w;
  if (glyph_width < 0) {
    int g = -glyph_width;
    (this->glyph())(glyph, x+w-g-3, y+((h-g)>>1), g, g, style, glyph_flags);
    lw = w-g-3;
  } else if (glyph_width > 0) {
    int g = glyph_width;
    (this->glyph())(glyph, x+3, y+((h-g)>>1), g, g, style, glyph_flags);
    lx = x+g+3; lw = w-g-3;
  }

  if (draw_label) this->draw_label(lx, y, lw, h, style, flags);

  if (focused())
    focusbox()->draw(x+1, y+1, w-2, h-2, style, flags|INVISIBLE);
}

void Button::draw() {
  if (type() == HIDDEN) {
    fl_did_clipping = this;
    return;
  }
  draw(0,0);
}

////////////////////////////////////////////////////////////////

static NamedStyle style("Button", 0, &Button::default_style);
NamedStyle* Button::default_style = &::style;

Button::Button(int x,int y,int w,int h, const char *l) : Widget(x,y,w,h,l) {
  style(default_style);
  //set_click_to_focus();
}

//
// End of "$Id: Fl_Button.cxx,v 1.66 2003/11/04 08:10:58 spitzak Exp $".
//

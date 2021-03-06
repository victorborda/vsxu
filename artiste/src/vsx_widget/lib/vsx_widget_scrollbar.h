/**
* Project: VSXu: Realtime visual programming language, music/audio visualizer, animation tool and much much more.
*
* @author Jonatan Wallmander, Robert Wenzel, Vovoid Media Technologies Copyright (C) 2003-2011
* @see The GNU Public License (GPL)
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program; if not, write to the Free Software Foundation, Inc.,
* 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#ifndef VSX_WIDGET_SCROLLBAR_H
#define VSX_WIDGET_SCROLLBAR_H


class vsx_widget_scrollbar : public vsx_widget {
  float scroll_handle_size;
  vsx_vector click_down;
  float cur_pos_click_down;
  float cur_pos;
  float value; // 
public:
  int scroll_type; // 0 = horizontal, 1 = vertical
  float scroll_window_size;
  float scroll_max; // scroll from 0 to what?
  float* control_value;
  vsx_widget_scrollbar();
  void event_mouse_move(vsx_widget_distance distance,vsx_widget_coords coords);
  void event_mouse_down(vsx_widget_distance distance,vsx_widget_coords coords,int button);

  float shz;
  void i_draw();

};

#endif

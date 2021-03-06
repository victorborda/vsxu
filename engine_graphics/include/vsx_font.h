/**
* Project: VSXu: Realtime visual programming language, music/audio visualizer, animation tool and much much more.
*
* @author Jonatan Wallmander, Vovoid Media Technologies Copyright (C) 2003-2011
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

#ifndef VSX_FONT_H
#define VSX_FONT_H

//#include "ftgl/FTGLExtrdFont.h"

//#include "ftgl/FTGLOutlineFont.h"
//#include "ftgl/FTGLPolygonFont.h"
#include "vsx_gl_global.h"
#include "vsx_math_3d.h"
#include "vsx_texture.h"

#include <vsx_platform.h>

#if PLATFORM_FAMILY == PLATFORM_FAMILY_UNIX
#define VSX_FONT_DLLIMPORT
#else
  #if defined(VSX_ENG_DLL)
    #define VSX_FONT_DLLIMPORT __declspec (dllexport)
  #else 
    #define VSX_FONT_DLLIMPORT __declspec (dllimport)
  #endif
#endif

struct vsx_font_info {
  int type; // 0 = texture (old) 1 = new (FtGL)
  vsx_texture* texture;  // pointer to either vsx_texture or ftgl font
#ifndef VSX_FONT_NO_FT
#ifndef VSX_NO_CLIENT
  FTFont* ftfont;
  FTFont* ftfont_outline;
#endif
#endif
};  

class vsx_font {
  static std::map<vsx_string, vsx_font_info*> glist;
  vsx_vector ep;
  void reinit(vsx_font_info* f_info,vsx_string font);
  bool list_built;
  GLuint dlist;
  //vsx_string old_string;
  //float old_size;
  float dx, dy, dz;
  vsx_string base_path;
  float ch, cw, size_s;
  float ddx,ddy;
  char* stc;
  char* colc;
  float sx,sy,ex,ey;

  void init_vars() {
    mode_2d = false;
    //old_size = 0;
    //dlist = glGenLists(1);
    list_built = false;
//    outline_transparency = 0;
    my_font_info = 0;
    //background = false;
    color.r = 1;
    color.g = 1;
    color.b = 1;
    color.a = 1;
    align = 1.0f;
  }
public:
  VSX_FONT_DLLIMPORT vsx_font_info* init(vsx_string font);

  vsx_avector<vsx_color> syntax_colors;
  // is in 2d mode?
  bool mode_2d;
  // keeping track of our font
  vsx_font_info* my_font_info;
  float outline_transparency;
  // is it translucent or not?
//  bool background;
  vsx_color color;
  float align;
  
  
  VSX_FONT_DLLIMPORT vsx_vector print(vsx_vector p, const vsx_string& str, const float size, const vsx_string colors = "");  
  VSX_FONT_DLLIMPORT vsx_vector print(vsx_vector p, const vsx_string& str, const vsx_string& font, float size, const vsx_string colors = "");  
  VSX_FONT_DLLIMPORT vsx_vector print_center(vsx_vector p, const vsx_string& str, float size);
  //vsx_vector print_center(vsx_vector p, const vsx_string& str, const vsx_string& font, float size);
  VSX_FONT_DLLIMPORT vsx_vector print_right(vsx_vector p, const vsx_string& str, float size);
  //vsx_vector print_right(vsx_vector p, const vsx_string& str, const vsx_string& font, float size);
  //vsx_vector get_size(vsx_vector p, const vsx_string& str, const vsx_string& font, float size);
  VSX_FONT_DLLIMPORT vsx_vector get_size(const vsx_string& str, float size);
  VSX_FONT_DLLIMPORT void reinit_all_active();
  
  vsx_font(vsx_string path) {
    base_path = path;
    init_vars();
  }

  vsx_font() {
    init_vars();
  }
};

#endif

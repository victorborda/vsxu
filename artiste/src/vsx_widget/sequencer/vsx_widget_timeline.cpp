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

#include <map>
#include <list>
#include <vector>
#include <math.h>
#include "vsx_gl_global.h"
#include "vsx_math_3d.h"
#include "vsx_texture_info.h"
#include "vsx_texture.h"
#include "vsx_command.h"
#include "vsx_font.h"
#include "vsxfst.h"
#include "vsx_mouse.h"
#include "vsx_param.h"
#include "vsx_module.h"
// local includes
#include "vsx_widget_base.h"
#include "lib/vsx_widget_lib.h"
#include "vsx_widget_sequence.h"
#include "vsx_widget_seq_chan.h"
#include "vsx_widget_timeline.h"
#include <vsx_command_client_server.h>
#include "server/vsx_widget_server.h"
// engine includes
#include "vsx_engine.h"


void vsx_widget_timeline::move_time(vsx_vector world) {
  if (owner->engine_status == VSX_ENGINE_STOPPED) {
    float f = (world.x+size.x/2)/(size.x);
    if (f < 0) f = 0;
    else
    if (f > 1) f = 1;
    float c_time = owner->tstart+f*(owner->tend - owner->tstart);
    if (c_time < 0) c_time = 0;

    float a = (c_time-owner->tstart)/(owner->tend-owner->tstart);
    //printf("a: %f\n",a);
    if (a > 0.95) {
      //dd_time = (0.001)*(owner->tend-owner->tstart);
      //owner->tend += dd_time;
      //owner->tstart += dd_time;
      //c_time = owner->tstart+(owner->tend-owner->tstart)*0.851;
      //owner->curtime + dd_time*2;
      auto_move_dir = 1;
      owner->update_time_from_engine = false;
      a_dist = a-0.95;
    } else
    if (a < 0.05) {
      //dd_time = (0.001)*(owner->tend-owner->tstart);
      //owner->tend -= dd_time;
      //owner->tstart -= dd_time;
      //c_time = owner->tstart+(owner->tend-owner->tstart)*0.149;
      auto_move_dir = -1;
      a_dist = 0.05-a;
      owner->update_time_from_engine = false;
    } else {
      auto_move_dir = 0;
      owner->curtime = c_time;
      command_q_b.add_raw("time_set "+f2s(owner->curtime));
      parent->vsx_command_queue_b(this);
      owner->update_time_from_engine = false;
    }
    //owner->check_timeline();
  } else {
    auto_move_dir = 0;
    owner->update_time_from_engine = true;
  }
}

void vsx_widget_timeline::i_draw() {
  parentpos = parent->get_pos_p();
	float time_diff = owner->tend - owner->tstart;
  totalsize = (owner->tend - owner->tstart);

  float y_mid = parentpos.y+pos.y;
  float y_size = size.y;
  if (a_focus == this) y_size *= 3.0f;
  float y_size_half = y_size * 0.5f;

  if (auto_move_dir) {
    //float c_time = owner->curtime;
    float curtime = owner->curtime + auto_move_dir*dtime*time_diff*10.0f*a_dist;//totalsize*0.01;
    float tstart = owner->tstart + auto_move_dir*dtime*time_diff*10.0f*a_dist;//totalsize*0.01;
    float tend = owner->tend + auto_move_dir*dtime*time_diff*10.0f*a_dist;//totalsize*0.01;
    if (curtime >= 0) {
      owner->curtime = curtime;
      owner->tstart = tstart;
      owner->tend = tend;
    }
    command_q_b.add_raw("time_set "+f2s(owner->curtime));
    parent->vsx_command_queue_b(this);
  }

 	glBegin(GL_QUADS);
    if (owner->tstart < 0) {
   	  glColor4f(0.4,0.3,0.3f,0.5f);
   	  ff = size.x*(fabs(owner->tstart)/totalsize);
  		glVertex2f(parentpos.x+pos.x-size.x*0.5f, y_mid+y_size_half);
  		glVertex2f(parentpos.x+pos.x-size.x*0.5f+ff, y_mid+y_size_half);
  		glVertex2f(parentpos.x+pos.x-size.x*0.5f+ff, y_mid-y_size_half);
  		glVertex2f(parentpos.x+pos.x-size.x*0.5f, y_mid-y_size_half);
      if (owner->tend > 0) {
    	  glColor4f(0.3,0.4,0.3,0.5f);
    		glVertex2f(parentpos.x+pos.x-size.x*0.5f+ff, y_mid+y_size_half);
    		glVertex2f(parentpos.x+pos.x+size.x*0.5f, y_mid+y_size_half);
    		glVertex2f(parentpos.x+pos.x+size.x*0.5f, y_mid-y_size_half);
    		glVertex2f(parentpos.x+pos.x-size.x*0.5f+ff, y_mid-y_size_half);
      }
    } else {
   	  glColor4f(0.3,0.4,0.3,0.5f);
  		glVertex2f(parentpos.x+pos.x-size.x*0.5f, y_mid+y_size_half);
  		glVertex2f(parentpos.x+pos.x+size.x*0.5f, y_mid+y_size_half);
  		glVertex2f(parentpos.x+pos.x+size.x*0.5f, y_mid-y_size_half);
  		glVertex2f(parentpos.x+pos.x-size.x*0.5f, y_mid-y_size_half);
    }
	glEnd();

  if (a_focus == this) {
 	  glColor3f(1.0f,1.0f,1.0f);
  } else
  glColor3f(0.5f,0.5f,0.5f);
  draw_box_border(vsx_vector(parentpos.x+pos.x-size.x*0.5,y_mid-y_size_half), vsx_vector(size.x,y_size), dragborder*0.5);

  levelstart = ((float)z_round(owner->tstart) - owner->tstart)/totalsize;
  //printf("levelstart: %f\n",levelstart);
  levelstart = 0;

  myf.color.a = 0.8f;

  float one_div_totalsize_times_sizex = 1.0f / totalsize * size.x;
	for (int i = (int)owner->tstart; i < (int)(owner->tend)+1; ++i)
	{
    glColor3f(0.5,0.5,0.5);
    float x =  (float)(i-owner->tstart) * one_div_totalsize_times_sizex;
    if (x > 0)
    {
      x += parentpos.x+pos.x - size.x*0.5f+levelstart*size.x;
      glBegin(GL_LINES);
        glVertex2f(x,y_mid+y_size*0.416666667f);
        glVertex2f(x,y_mid-y_size*0.416666667f);
      glEnd();
      myf.print_center(vsx_vector(x,y_mid), i2s(i),0.005);
    }
	}

  glColor3f(1,1,1);
  float f = ((owner->curtime-owner->tstart)/(owner->tend-owner->tstart))*size.x;
  glBegin(GL_LINES);
    glVertex2f(parentpos.x+pos.x-size.x*0.5f+f,y_mid+y_size*0.416666667);
    glVertex2f(parentpos.x+pos.x-size.x*0.5f+f,y_mid-y_size*0.416666667);
  glEnd();

  // ************************************************************
  // sound waveform display
	// ************************************************************
  if (show_wave_data)
  {
		vsx_widget_server* server = (vsx_widget_server*)owner->get_server();
		vsx_engine* engine = (vsx_engine*)(server->engine);
    vsx_module_engine_info* engine_info = engine->get_engine_info();

    if (engine_info->param_float_arrays.size() >= 4 && a_focus == this)
		{
			vsx_engine_float_array *full_pcm_data_l;
			vsx_engine_float_array *full_pcm_data_r;
      full_pcm_data_l = engine_info->param_float_arrays[2];
      full_pcm_data_r = engine_info->param_float_arrays[3];
			if (full_pcm_data_l->array.size() > 0)
			{
				// assuming we have 44100 samples per second
				float x_start = parentpos.x+pos.x-size.x*0.5f;
				float x_end   = parentpos.x+pos.x+size.x*0.5f;
				float t_start = owner->tstart;
				float t_end = owner->tend;
				size_t data_end = (size_t) (t_end * 44100.0f);
				if (owner->tstart < 0)
				{
					x_start += fabs(t_start / (t_end - t_start)) * size.x;
					//data_end -= fabs(t_start) * 44100.0f;
					t_start = 0.0f;
				}
				size_t data_start = (size_t) (t_start * 44100.0f);

				size_t data_count = data_end - data_start;
				float x_dist = x_end - x_start;
				double x_diff = (double)x_dist / (double)data_count;

	//			printf("data_start: %d\n", data_start);
				//printf("data_end: %d\n", data_end);
				//printf("data_count: %d\n", data_count);

				glColor4f(1.0f,0.2f,0.2f,0.15f);
				double x_pos = x_start;
				glBegin(GL_LINE_STRIP);
					for (
								size_t i = data_start;
								i < data_end;
								i++
							)
					{
						glVertex2f(x_pos, y_mid + (*full_pcm_data_l).array[i] * y_size_half );
						x_pos += x_diff;
					}
				glEnd();


				glColor4f(0.2f,1.0f,0.2f,0.1f);
				x_pos = x_start;
				glBegin(GL_LINE_STRIP);
					for (
								size_t i = data_start;
								i < data_end;
								i++
							)
					{
						glVertex2f(x_pos, y_mid + (*full_pcm_data_r).array[i] * y_size_half );
						x_pos += x_diff;
					}
				glEnd();
			} // pcm data pool size
		}
  }

	vsx_widget::i_draw();
}

bool vsx_widget_timeline::event_key_down(signed long key, bool alt, bool ctrl, bool shift) {
  float dt = (owner->tend-owner->tstart)*0.5;
  switch(key) {
		case 'a':
		{
			if (ctrl)
				backwards_message("play");
			else
				backwards_message("stop");
		}
  }

  if (key == 't') show_wave_data = !show_wave_data;

  if (!ctrl) return true;
  switch(key) {
    case 'f':
      {
        owner->tstart += dt*0.03;
        owner->tend += dt*0.03;
        return false;
      }
    break;
    case 's':
      {
        owner->tstart -= dt*0.03;
        owner->tend -= dt*0.03;
        return false;
      }
    break;
    case 'w':
      {
        if (owner->curtime < owner->tstart) {
          owner->tstart = owner->curtime;
          owner->tend = owner->curtime + dt*2;
        } else
        if (owner->curtime > owner->tend) {
          owner->tstart = owner->curtime-dt*2;
          owner->tend = owner->curtime;
        }
        owner->tstart -= (owner->curtime-owner->tstart)/(dt)*dt*0.03;
        owner->tend += (owner->tend-owner->curtime)/(dt)*dt*0.03;
        return false;
      }
    break;
    case 'r':
      {

        if (owner->curtime < owner->tstart || owner->curtime > owner->tend) {
          //ddt = (owner->tend - owner->tstart)*0.5;
          owner->tstart = owner->curtime - dt;
          owner->tend = owner->curtime + dt;
        }
        //float ndt = dt*1.1;



        //float _tstart = owner->tstart+dt-dt*1.1;
        owner->tstart += (owner->curtime-owner->tstart)/(dt)*dt*0.03;
        owner->tend -= (owner->tend-owner->curtime)/(dt)*dt*0.03;
        //float _tend = owner->tend-dt+dt*1.1;
        //owner->tstart = owner->tstart+dt-dt*1.1;
        //owner->tend = owner->tend-dt+dt*1.1;
        //if (_tend - _tstart < 40) {
          //owner->tstart = _tstart;
          //owner->tend = _tend;


//          float dt = (owner->tend-owner->tstart)*0.5;
        /*float _tstart = owner->tstart+dt-dt*0.9;
        float _tend = owner->tend-dt+dt*0.9;
        if (owner->curtime > _tstart) {
          owner->tstart = _tstart;
          owner->tend = _tend;
        } */
        return false;
      }
    break;
  }
  return true;
}

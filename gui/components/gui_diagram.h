#ifndef gui_diagram_h
#define gui_diagram_h

#include "../../simtypes.h"
#include "gui_component.h"
#include "../../tpl/slist_tpl.h"
#include "../../linehandle_t.h"
#include "gui_label.h"
#include "gui_button.h"
#include "../gui_frame.h"
#include "../../simline.h"
#include "../../dataobj/diagram.h"
//class gui_component_t;
class gui_diagram_t :
	public gui_container_t,
	public action_listener_t
{
 public:
	gui_diagram_t(linehandle_t line);
	gui_diagram_t();
	~gui_diagram_t(){};
	void set_background(int color);
	void draw(scr_coord offset);
	void set_dimension(int x, int y) {
		x_elements = x;
		y_elements = y;
	}
	void set_line(linehandle_t line_){line=line_;std::cout<<"called set_line()"<<std::endl;}
	void update_diagram();
	void register_diagram();
	void draw_diagram();
	bool action_triggered(gui_action_creator_t*, value_t) OVERRIDE;
	void horizontal_zoom(bool magnify);
	void vertical_zoom(bool magnify);
	int* get_horizontal_zoom_factor(){
		return horizontal_zoom_factor;
	}
	int* get_vertical_zoom_factor(){
		return vertical_zoom_factor;
	}
	
private:
	int x_elements, y_elements;
	//for GUI
	scr_coord tooltipcoord;
	scr_size original_size;
	bool show_x_axis, show_y_axis, ltr;
 	const player_t *owner;
	int background;
	int station_label_length;
	int month_width;
	int station_label_left_margin;
	int station_label_top_margin;
	int horizontal_zoom_factor[2];
	int vertical_zoom_factor[2];
	gui_label_t l_station_header;
	slist_tpl<gui_label_t*> l_hour_header;
	slist_tpl<gui_label_t*> l_station_name;
	//for core
	linehandle_t line;
	schedule_t* schedule;
	diagram_t diagram;
};

#endif

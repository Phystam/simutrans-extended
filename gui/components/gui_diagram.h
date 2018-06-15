#ifndef gui_diagram_h
#define gui_diagram_h

#include "../../simtypes.h"
#include "gui_component.h"
#include "../../tpl/slist_tpl.h"
#include "gui_label.h"
#include "gui_button.h"
#include "../gui_frame.h"
//class gui_component_t;
class gui_diagram_t :
	public gui_container_t,
	public action_listener_t
{
 public:
	gui_diagram_t();
	~gui_diagram_t(){};
	void set_background(int color);
	void draw(scr_coord offset);
	void set_dimension(int x, int y) {
		x_elements = x;
		y_elements = y;
	}

	void update_diagram();
	bool action_triggered(gui_action_creator_t*, value_t) OVERRIDE;
	
 private:
	int x_elements, y_elements;

	scr_coord tooltipcoord;

	bool show_x_axis, show_y_axis, ltr;

	int background;

	gui_label_t l_station_header;
	slist_tpl<gui_label_t*> l_hour_header;
	slist_tpl<gui_label_t*> l_station_name;
};

#endif

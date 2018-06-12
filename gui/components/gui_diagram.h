#ifndef gui_diagram_h
#define gui_diagram_h

#include "../../simtypes.h"
#include "gui_component.h"
#include "../../tpl/slist_tpl.h"

class gui_diagram_t : public gui_component_t
{
 public:
	gui_diagram_t();
	void set_background(int color);
	void draw(scr_coord offset);
	void set_dimension(int x, int y) {
		x_elements = x;
		y_elements = y;
	}

	void set_seed(int seed) { this->seed = seed; }


	
 private:
	int x_elements, y_elements;

	int seed;

	scr_coord tooltipcoord;

	bool show_x_axis, show_y_axis, ltr;

	int background;

};

#endif

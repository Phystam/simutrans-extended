#include <string.h>
#include <cmath>

#include "gui_diagram.h"
#include "../gui_frame.h"
#include "../simwin.h"
#include "../../simcolor.h"
#include "../../utils/simstring.h"
#include "../../dataobj/environment.h"
#include "../../display/simgraph.h"
#include "../../display/viewport.h"
#include "../gui_theme.h"
#include "../../simworld.h"
#include "gui_label.h"
#include "gui_component.h"
#include "../gui_frame.h"
gui_diagram_t::gui_diagram_t() : l_station_header("stations")
{
	tooltipcoord = scr_coord::invalid;

	show_x_axis = true;
	show_y_axis = true;
	ltr = false;
	x_elements = 0;

	//transparent by default
	background = COL_WHITE;
	scr_coord cursor(0,0);
  l_station_header.set_pos(cursor);
	l_station_header.set_color(SYSCOL_TEXT);
	add_component(&l_station_header);

	update_diagram();
}

void gui_diagram_t::draw(scr_coord offset)
{
	offset += pos;
	sint64 baseline = 0;
	sint64* pbaseline = &baseline;
	float scale = 0;
	float* pscale = &scale;

	sint64 time=0;
	sint64 distance=0;
	
	
	if(background != -1) {
		display_fillbox_wh_clip(offset.x, offset.y, size.w, size.h, background, false);
	}

	//draw diagram frame
	display_ddd_box_clip(offset.x, offset.y, size.w, size.h, SYSCOL_SHADOW, SYSCOL_HIGHLIGHT);

	display_direct_line(offset.x+1, offset.y+(scr_coord_val)baseline, offset.x+size.w-2, offset.y+(scr_coord_val)baseline, MN_GREY4);

	//horizontal line
	display_direct_line(offset.x, offset.y+(scr_coord_val)baseline+12, offset.x+size.w  , offset.y+(scr_coord_val)baseline+12, COL_BLACK);

	//vertical line
	display_direct_line(offset.x+30, offset.y+(scr_coord_val)baseline+12, offset.x+30  , offset.y+(scr_coord_val)baseline+size.h, COL_BLACK);

}

void gui_diagram_t::set_background(int i)
{
	background = i;
}

void gui_diagram_t::update_diagram() {


}

bool gui_diagram_t::action_triggered(gui_action_creator_t *comp, value_t extra)
{
	if (extra.i & ~1) {
		koord k = *(const koord *)extra.p;
		world()->get_viewport()->change_world_position(koord3d(k, world()->max_hgt(k)));
	}

	return true;
}

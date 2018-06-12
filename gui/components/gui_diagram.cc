#include <string.h>
#include <cmath>

#include "gui_diagram.h"
#include "../gui_frame.h"
#include "../simwin.h"
#include "../../simcolor.h"
#include "../../utils/simstring.h"
#include "../../dataobj/environment.h"
#include "../../display/simgraph.h"
#include "../gui_theme.h"

gui_diagram_t::gui_diagram_t() : gui_component_t()
{
	tooltipcoord = scr_coord::invalid;

	seed = 0;
	show_x_axis = true;
	show_y_axis = true;
	ltr = false;
	x_elements = 0;

	// Hajo: transparent by default
	background = COL_WHITE;

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

	// draw diagram frame
	display_ddd_box_clip(offset.x, offset.y, size.w, size.h, SYSCOL_SHADOW, SYSCOL_HIGHLIGHT);

	display_direct_line(offset.x+1, offset.y+(scr_coord_val)baseline, offset.x+size.w-2, offset.y+(scr_coord_val)baseline, MN_GREY4);

}

void gui_diagram_t::set_background(int i)
{
	background = i;
}

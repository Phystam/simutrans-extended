#include "../simworld.h"
#include "../simhalt.h"
#include "../simline.h"
#include "../dataobj/schedule.h"
#include "../display/viewport.h"
#include "../dataobj/translator.h"
#include "times_history.h"

#include "diagram_visualizer.h"

/*
	This class is made by Phystam
*/

diagram_visualizer_t::diagram_visualizer_t(linehandle_t line_, convoihandle_t convoi_) :
	gui_frame_t(NULL, line_.is_bound() ? line_->get_owner() : convoi_->get_owner()),
	owner(line_.is_bound() ? line_->get_owner() : convoi_->get_owner()),
	line(line_),
	convoi(convoi_),
	scrolly(&cont)
{
	set_name(title_buf);

	scrolly.set_pos(scr_coord(0, 0));
	scrolly.set_show_scroll_x(true);
	add_component(&scrolly);

	set_windowsize(scr_size(D_DEFAULT_WIDTH, D_TITLEBAR_HEIGHT+4+22*(LINESPACE)+D_SCROLLBAR_HEIGHT+2));
	set_min_windowsize(scr_size(D_DEFAULT_WIDTH, D_TITLEBAR_HEIGHT+4+3*(LINESPACE)+D_SCROLLBAR_HEIGHT+2));

	set_resizemode(diagonal_resize);
	resize(scr_coord(0,0));
}



diagram_visualizer_t::~diagram_visualizer_t()
{
	//do nothing
}

void diagram_visualizer_t::set_windowsize(scr_size size)
{
	gui_frame_t::set_windowsize(size);
	scrolly.set_size(get_client_windowsize()-scrolly.get_pos());
}


diagram_visualizer_t::diagram_visualizer_t():
	gui_frame_t("", NULL),
	scrolly(&cont)
{
	// just a dummy
}

void diagram_visualizer_t::update_components(){
		const char *name;
	title_buf.clear();
	title_buf.append(translator::translate("diagram_visualizer_window"));
	name = line->get_name();
	title_buf.append(name);
}

bool diagram_visualizer_t::action_triggered(gui_action_creator_t *comp, value_t extra)
{
	return true;
}

void diagram_visualizer_t::draw(scr_coord pos, scr_size size)
{
	gui_frame_t::draw( pos, size );
	//just a dummy
}

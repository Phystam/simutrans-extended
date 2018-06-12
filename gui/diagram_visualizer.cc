#include "../simworld.h"
#include "../simhalt.h"
#include "../simline.h"
#include "../dataobj/schedule.h"
#include "../display/viewport.h"
#include "../dataobj/translator.h"
#include "times_history.h"

#include "diagram_visualizer.h"
#define LINE_NAME_COLUMN_WIDTH ((D_BUTTON_WIDTH*3)+11+4)
#define SCL_HEIGHT (15*LINESPACE)
#define L_BUTTON_WIDTH (button_size.w)

/*
	This class is made by Phystam
*/

diagram_visualizer_t::diagram_visualizer_t(linehandle_t line_, convoihandle_t convoi_) :
	gui_frame_t(NULL, line_.is_bound() ? line_->get_owner() : convoi_->get_owner()),
	owner(line_.is_bound() ? line_->get_owner() : convoi_->get_owner()),
	line(line_),
	convoi(convoi_),
	scrolly(&cont),
	hzoom_label("horizontal zoom"),
	vzoom_label("vertical zoom")
{
	//component position cursor and some constants from map_frame.cc
	scr_coord cursor( D_MARGIN_LEFT,D_MARGIN_TOP ); 
	const scr_coord_val zoom_label_width = display_get_char_max_width("0123456789") * 4 + display_get_char_width(':');
	const scr_size button_size(max(D_BUTTON_WIDTH, 100), D_BUTTON_HEIGHT);

	update_components();
	set_name(title_buf);

	// diagram visualizer window
	scrolly.set_pos(scr_coord(0, 0));
	scrolly.set_show_scroll_x(true);
	add_component(&scrolly);
	set_windowsize(scr_size(D_DEFAULT_WIDTH, D_TITLEBAR_HEIGHT+4+22*(LINESPACE)+D_SCROLLBAR_HEIGHT+2));
	set_min_windowsize(scr_size(D_DEFAULT_WIDTH, D_TITLEBAR_HEIGHT+4+3*(LINESPACE)+D_SCROLLBAR_HEIGHT+2));
	set_resizemode(diagonal_resize);
	resize(scr_coord(0,0));

	//	append schedule button
	
	b_append_schedule.init(button_t::roundbox_state, "Append schedule", cursor, button_size);
	b_append_schedule.set_tooltip("Append schedule");
	b_append_schedule.set_visible(true);
	b_append_schedule.add_listener(this);
	add_component(&b_append_schedule);
	cursor.x += L_BUTTON_WIDTH+D_H_SPACE;

	b_clear_schedule.init(button_t::roundbox_state, "Clear schedule", cursor, button_size);
	b_clear_schedule.set_tooltip("Clear schedule");
	b_clear_schedule.set_visible(true);
	b_clear_schedule.add_listener(this);
	add_component(&b_clear_schedule);
	cursor = scr_coord(D_MARGIN_LEFT, cursor.y + D_BUTTON_HEIGHT + D_V_SPACE);

	// zool label
	vzoom_label.set_pos(cursor);
	vzoom_label.set_color(SYSCOL_TEXT);
	add_component( &vzoom_label );
	cursor.x += vzoom_label.get_size().w + D_H_SPACE;

	//vertical zoom arrow [<] 1:1 [>]
	zoom_buttons[0].init(button_t::repeatarrowleft, NULL,cursor);
	zoom_buttons[0].add_listener( this );
	add_component( zoom_buttons+0 );
	cursor.x += zoom_buttons[0].get_size().w;

	vzoom_value_label.set_pos(cursor);
	vzoom_value_label.set_size( scr_size(zoom_label_width,LINESPACE) );
	vzoom_value_label.set_align(gui_label_t::centered);
	vzoom_value_label.set_color(SYSCOL_TEXT);
	add_component( &vzoom_value_label );
	cursor.x += zoom_label_width;

	zoom_buttons[1].init(button_t::repeatarrowright, NULL,cursor);
	zoom_buttons[1].add_listener( this );
	add_component( zoom_buttons+1 );
	cursor.x += zoom_buttons[0].get_size().w + D_H_SPACE;

	//horizontal zoom 
	hzoom_label.set_pos(cursor);
	hzoom_label.set_color(SYSCOL_TEXT);
	add_component( &hzoom_label );
	cursor.x += hzoom_label.get_size().w + D_H_SPACE;

	zoom_buttons[2].init(button_t::repeatarrowleft, NULL,cursor);
	zoom_buttons[2].add_listener( this );
	add_component( zoom_buttons+2 );
	cursor.x += zoom_buttons[2].get_size().w;

	hzoom_value_label.set_pos(cursor);
	hzoom_value_label.set_size( scr_size(zoom_label_width,LINESPACE) );
	hzoom_value_label.set_align(gui_label_t::centered);
	hzoom_value_label.set_color(SYSCOL_TEXT);
	add_component( &hzoom_value_label );
	cursor.x += zoom_label_width;

	zoom_buttons[3].init(button_t::repeatarrowright, NULL,cursor);
	zoom_buttons[3].add_listener( this );
	add_component( zoom_buttons+3 );
	cursor= scr_coord(0, cursor.y + D_BUTTON_HEIGHT + D_V_SPACE);

	//diagram
	
	diagram.set_pos(cursor);
	diagram.set_size(scr_size(1000,1000));
	diagram.set_background(COL_WHITE);
	diagram.set_visible(true);
	//	diagram.add_listener(this);
	add_component(&diagram);


	
	draw(scr_coord(0,0), scr_size(D_DEFAULT_WIDTH, D_TITLEBAR_HEIGHT+4+22*(LINESPACE)+D_SCROLLBAR_HEIGHT+2));
	
}



diagram_visualizer_t::~diagram_visualizer_t()
{
	//do nothing
}

void diagram_visualizer_t::set_windowsize(scr_size size)
{
	gui_frame_t::set_windowsize(size);
	scrolly.set_size(get_client_windowsize()-scrolly.get_pos());
	draw(scr_coord(0,0),scr_size(100,100));
	update_lineinfo();

}


diagram_visualizer_t::diagram_visualizer_t():
	gui_frame_t("", NULL),
	scrolly(&cont)
{
	draw(scr_coord(0,0),scr_size(100,100));
	update_lineinfo();
	// just a dummy
}

void diagram_visualizer_t::update_components(){
	const char *name;
	title_buf.clear();
	title_buf.append(translator::translate("Diagram Visualizer: "));
	name = line->get_name();
	title_buf.append(name);
}

bool diagram_visualizer_t::action_triggered(gui_action_creator_t *comp, value_t extra)
{
	return true;
}

void diagram_visualizer_t::draw(scr_coord pos, scr_size size)
{

	if(line.is_bound()){
		if ((!line->get_schedule()->empty() && !line->get_schedule()->matches(welt, last_schedule))) {
			register_containers();
		}
	}
	// diagram.set_dimension(120, 1000);
	// diagram.set_pos( scr_coord(0,0) );
	// diagram.set_seed(0);
	// diagram.set_background(COL_WHITE);
	// add_component(&diagram);
	
	gui_frame_t::draw( pos, size );
	//just a dummy
}
	
void diagram_visualizer_t::update_lineinfo()
{
	diagram.set_visible(true);
}

void diagram_visualizer_t::register_containers()
{
}

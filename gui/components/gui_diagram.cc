#include <string.h>
#include <cmath>
#include <iostream>

#include "gui_diagram.h"

#include "../../simline.h"
#include "../../simhalt.h"
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

gui_diagram_t::gui_diagram_t(linehandle_t line_) :
	line(line_)
{
	tooltipcoord = scr_coord::invalid;

	show_x_axis = true;
	show_y_axis = true;
	ltr = false;
	x_elements = 0;
	station_label_left_margin=4;
	station_label_top_margin=4;

	//transparent by default
	background = COL_WHITE;
	scr_coord cursor(0,0);
  l_station_header.set_pos(cursor);
	l_station_header.set_color(SYSCOL_TEXT);
	add_component(&l_station_header);

	//setting of zoom factor, [0] is scale down, [1] is magnification
	horizontal_zoom_factor[0]=1;
	horizontal_zoom_factor[1]=1;
	
	vertical_zoom_factor[0]=1;
	vertical_zoom_factor[1]=1;
	std::cout<<"i am constractor"<< size.w<<std::endl;
	
	update_diagram();
	
	std::cout << "size="<<line.get_size()<<std::endl;

}

gui_diagram_t::gui_diagram_t(){
	tooltipcoord = scr_coord::invalid;

	show_x_axis = true;
	show_y_axis = true;
	ltr = false;
	x_elements = 0;
	station_label_left_margin=4;
	station_label_top_margin=4;

	//transparent by default
	background = COL_WHITE;
	scr_coord cursor(0,0);
  l_station_header.set_pos(cursor);
	l_station_header.set_color(SYSCOL_TEXT);
	add_component(&l_station_header);

	//setting of zoom factor, [0] is scale down, [1] is magnification
	horizontal_zoom_factor[0]=1;
	horizontal_zoom_factor[1]=1;
	
	vertical_zoom_factor[0]=1;
	vertical_zoom_factor[1]=1;
	std::cout<<"i am constractor"<< size.w<<std::endl;
	
	update_diagram();

}

void gui_diagram_t::draw(scr_coord offset)
{
	update_diagram();
	uint32 station_number = diagram.get_station_name_list().get_size() - 1;
	offset += pos;
	sint64 baseline = 0;
	sint64* pbaseline = &baseline;
	float scale = 0;
	float* pscale = &scale;
	int width_hour = proportional_string_width("00:00");
	for(int i=0;i<station_number;i++){
		uint32 station_label_length_temp = proportional_string_width(diagram.get_station_name_list()[i])+station_label_left_margin+width_hour;
		if(station_label_length<station_label_length_temp){
			station_label_length=station_label_length_temp;
		}
	}
	uint32 month_length = world()->ticks_to_seconds(world()->ticks_per_world_month);
	original_size.w=month_length/10+station_label_length;
	month_width=month_length/10;
	sint64 time=0;
	sint64 distance=500;
	
	size.h = (scr_coord_val)baseline + 12 + station_label_top_margin + diagram.get_total_distance_list()[station_number-1]/10;
	if(background != -1) {
		display_fillbox_wh_clip(offset.x, offset.y, size.w, size.h, background, false);
	}


	//vertical line
	//1mins line

	for(int imin=0;imin*6<month_width;imin++){
		
		display_direct_line(offset.x + station_label_length + imin*6 * horizontal_zoom_factor[0]/horizontal_zoom_factor[1], offset.y+(scr_coord_val)baseline+12+station_label_top_margin + 1, offset.x + station_label_length + imin*6* horizontal_zoom_factor[0]/horizontal_zoom_factor[1]  , offset.y+(scr_coord_val)baseline+size.h, MN_GREY4);
	}
	//10mins line
	for(int imin=0;imin*60<month_width;imin++){
		display_direct_line(offset.x + station_label_length + imin*60* horizontal_zoom_factor[0]/horizontal_zoom_factor[1], offset.y+(scr_coord_val)baseline + 12 + station_label_top_margin + 1, offset.x + station_label_length + imin*60* horizontal_zoom_factor[0]/horizontal_zoom_factor[1]  , offset.y+(scr_coord_val)baseline+size.h, MN_GREY2);
	}
	//hour line

	for(int ihour=0;ihour*360<month_width;ihour++){
		//line
		display_direct_line(offset.x + station_label_length + ihour*360* horizontal_zoom_factor[0]/horizontal_zoom_factor[1], offset.y+(scr_coord_val)baseline + 12 + station_label_top_margin + 1, offset.x + station_label_length + ihour*360* horizontal_zoom_factor[0]/horizontal_zoom_factor[1]  , offset.y+(scr_coord_val)baseline+size.h, MN_GREY0);
		//hour label
		char hour_label[10];
		sprintf(hour_label, "%02d:00", ihour);
		display_proportional_clip( offset.x + station_label_length + ihour*360* horizontal_zoom_factor[0]/horizontal_zoom_factor[1] - width_hour/2, offset.y + station_label_top_margin, hour_label, ALIGN_LEFT, COL_BLACK, true );

	}
	//left vertical line
	display_direct_line(offset.x+station_label_length, offset.y+(scr_coord_val)baseline + station_label_top_margin + 12, offset.x + station_label_length  , offset.y+(scr_coord_val)baseline+size.h, COL_BLACK);

	//-----time part end -----------
	//-----station part start ------


	//horizontal line
	
	for(uint32 i=0;i<station_number;i++){
		display_direct_line(offset.x, offset.y+(scr_coord_val)baseline + 12 + station_label_top_margin + diagram.get_total_distance_list()[i]/10, offset.x+size.w, offset.y+(scr_coord_val)baseline + 12 + station_label_top_margin + diagram.get_total_distance_list()[i]/10, COL_BLACK);
		display_proportional_clip( offset.x + station_label_left_margin , offset.y + station_label_top_margin + diagram.get_total_distance_list()[i]/10, diagram.get_station_name_list()[i], ALIGN_LEFT, COL_BLACK, true );

		
	}

	display_direct_line(offset.x, offset.y+(scr_coord_val)baseline+size.h, offset.x+size.w  , offset.y+(scr_coord_val)baseline+size.h, COL_BLACK);

	//draw diagram frame
	display_ddd_box_clip(offset.x, offset.y, size.w, size.h, SYSCOL_SHADOW, SYSCOL_HIGHLIGHT);

	// display_direct_line(offset.x+1, offset.y+(scr_coord_val)baseline, offset.x+size.w-2, offset.y+(scr_coord_val)baseline, MN_GREY4);

	scr_coord label_position(offset.x,offset.y);

	draw_diagram();
	//	scr_coord p = karte_to_screen( stadt->get_pos() );
	//	label_position.x = clamp( label_position.x, 0, get_size().w-w );
	//	p += pos;
}

void gui_diagram_t::set_background(int i)
{
	background = i;
}

void gui_diagram_t::update_diagram() {
	diagram.set_diagram_entries_from(line);
}

bool gui_diagram_t::action_triggered(gui_action_creator_t *comp, value_t extra)
{
	return true;
}

void gui_diagram_t::horizontal_zoom(bool magnify){
	if(magnify){
		if(	horizontal_zoom_factor[0]!=1){
			horizontal_zoom_factor[0]--;
		}else if( horizontal_zoom_factor[0]==1&&horizontal_zoom_factor[1]<6){
			horizontal_zoom_factor[1]++;
		}
	}	else {//scale down
		if(	horizontal_zoom_factor[1]==1&&horizontal_zoom_factor[0]<4){
			horizontal_zoom_factor[0]++;
		}else if( horizontal_zoom_factor[1]!=1){
			horizontal_zoom_factor[1]--;
		}
	}
	size.w = ( original_size.w-station_label_length ) * horizontal_zoom_factor[0]/horizontal_zoom_factor[1] + station_label_length;
}
void gui_diagram_t::vertical_zoom(bool magnify){
	if(magnify){
		if(	vertical_zoom_factor[0]!=1){
			vertical_zoom_factor[0]--;
		}else if( vertical_zoom_factor[0]==1&&vertical_zoom_factor[1]<6){
			vertical_zoom_factor[1]++;
		}
	}	else {//scale down
		if(	vertical_zoom_factor[1]==1&&vertical_zoom_factor[0]<4){
			vertical_zoom_factor[0]++;
		}else if( vertical_zoom_factor[1]!=1){
			vertical_zoom_factor[1]--;
		}
	}

}

void gui_diagram_t::register_diagram(){

}
void gui_diagram_t::draw_diagram(){
	
}

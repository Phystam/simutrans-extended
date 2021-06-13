/*
 * This file is part of the Simutrans-Extended project under the Artistic License.
 * (see LICENSE.txt)
 */

#include "trafficlight_info.h"
#include "components/gui_label.h"
#include "../obj/roadsign.h" // The rest of the dialog

#include "../simmenu.h"
#include "../simworld.h"

trafficlight_info_t::trafficlight_info_t(roadsign_t* s) :
	obj_infowin_t(s),
	roadsign(s)
{
	add_table(3,1);
	{
	  ns.set_limits( 1, 255 );
	  ns.set_value( s->get_ticks_ns() );
	  ns.wrap_mode( false );
	  ns.add_listener( this );
	  add_component( &ns );
	  
	  ow.set_limits( 1, 255 );
	  ow.set_value( s->get_ticks_ow() );
	  ow.wrap_mode( false );
	  ow.add_listener( this );
	  add_component( &ow );
	  
	  offset.set_limits( 0, 255 );
	  offset.set_value( s->get_ticks_offset() );
	  offset.wrap_mode( false );
	  offset.add_listener( this );
	  add_component( &offset );
	}
	end_table();

	add_table(2,1);
	{
	  amber_ns.set_limits( 1, 255 );
	  amber_ns.set_value( s->get_ticks_amber_ns() );
	  amber_ns.wrap_mode( false );
	  amber_ns.add_listener( this );
	  add_component( &amber_ns );
	  
	  amber_ow.set_limits( 1, 255 );
	  amber_ow.set_value( s->get_ticks_amber_ow() );
	  amber_ow.wrap_mode( false );
	  amber_ow.add_listener( this );
	  add_component( &amber_ow );
	}
	end_table();

	// show author below the settings
	if (char const* const maker = roadsign->get_desc()->get_copyright()) {
		gui_label_buf_t* lb = new_component<gui_label_buf_t>();
		lb->buf().printf(translator::translate("Constructed by %s"), maker);
		lb->update();
	}

	recalc_size();
}


/**
 * This method is called if an action is triggered
 *
 * Returns true, if action is done and no more
 * components should be triggered.
 */
bool trafficlight_info_t::action_triggered( gui_action_creator_t *comp, value_t v)
{
	set_tool_at(comp, v, roadsign->get_pos());
	return true;
}


// notify for an external update
void trafficlight_info_t::update_data()
{
	ns.set_value( roadsign->get_ticks_ns() );
	ow.set_value( roadsign->get_ticks_ow() );
	offset.set_value( roadsign->get_ticks_offset() );
	amber_ns.set_value( roadsign->get_ticks_amber_ns() );
	amber_ow.set_value( roadsign->get_ticks_amber_ow() );
}

void trafficlight_info_t::set_tool_at( gui_action_creator_t* comp, value_t v, koord3d k)
{
  char param[256];
  if(comp == &ns) {
    sprintf( param, "%s,1,%i", k.get_str(), (int)v.i );
    tool_t::simple_tool[TOOL_CHANGE_TRAFFIC_LIGHT]->set_default_param( param );
    welt->set_tool( tool_t::simple_tool[TOOL_CHANGE_TRAFFIC_LIGHT], welt->get_active_player() );
    //check the neighbouring traffic light
    for(uint8 i=0; i<4; i++){
      const koord3d neighbour = k + koord3d(koord::nesw[i],0);
      grund_t* gr = welt->lookup(neighbour);
      const roadsign_t* rs = gr->find<roadsign_t>();
      if( rs && rs->get_desc()->is_traffic_light() && welt->get_active_player() == rs->get_owner() ){
	if( rs->get_ticks_ns() != (uint8)v.i ){
	  set_tool_at(comp, v, neighbour);
	}
      }
    }
  }
  else if(comp == &ow) {
    sprintf( param, "%s,0,%i", k.get_str(), (int)v.i );
    tool_t::simple_tool[TOOL_CHANGE_TRAFFIC_LIGHT]->set_default_param( param );
    welt->set_tool( tool_t::simple_tool[TOOL_CHANGE_TRAFFIC_LIGHT], welt->get_active_player() );
    for(uint8 i=0; i<4; i++){
      const koord3d neighbour = k + koord3d(koord::nesw[i],0);
      grund_t* gr = welt->lookup(neighbour);
      const roadsign_t* rs = gr->find<roadsign_t>();
      if( rs && rs->get_desc()->is_traffic_light() && welt->get_active_player() == rs->get_owner() ){
	if( rs->get_ticks_ow() != (uint8)v.i ){
	  set_tool_at(comp, v, neighbour);
	}
      }
    }
  }
  else if(comp == &offset) {
    sprintf( param, "%s,2,%i", k.get_str(), (int)v.i );
    tool_t::simple_tool[TOOL_CHANGE_TRAFFIC_LIGHT]->set_default_param( param );
    welt->set_tool( tool_t::simple_tool[TOOL_CHANGE_TRAFFIC_LIGHT], welt->get_active_player() );
    for(uint8 i=0; i<4; i++){
      const koord3d neighbour = k + koord3d(koord::nesw[i],0);
      grund_t* gr = welt->lookup(neighbour);
      const roadsign_t* rs = gr->find<roadsign_t>();
      if( rs && rs->get_desc()->is_traffic_light() && welt->get_active_player() == rs->get_owner() ){
	if( rs->get_ticks_offset() != (uint8)v.i ){
	  set_tool_at(comp, v, neighbour);
	}
      }
    }
  }
  else if(comp == &amber_ns) {
    sprintf( param, "%s,4,%i", k.get_str(), (int)v.i );
    tool_t::simple_tool[TOOL_CHANGE_TRAFFIC_LIGHT]->set_default_param( param );
    welt->set_tool( tool_t::simple_tool[TOOL_CHANGE_TRAFFIC_LIGHT], welt->get_active_player() );
    for(uint8 i=0; i<4; i++){
      const koord3d neighbour = k + koord3d(koord::nesw[i],0);
      grund_t* gr = welt->lookup(neighbour);
      const roadsign_t* rs = gr->find<roadsign_t>();
      if( rs && rs->get_desc()->is_traffic_light() && welt->get_active_player() == rs->get_owner() ){
	if( rs->get_ticks_amber_ns() != (uint8)v.i ){
	  set_tool_at(comp, v, neighbour);
	}
      }
    }
  }
  else if(comp == &amber_ow) {
    sprintf( param, "%s,3,%i", k.get_str(), (int)v.i );
    tool_t::simple_tool[TOOL_CHANGE_TRAFFIC_LIGHT]->set_default_param( param );
    welt->set_tool( tool_t::simple_tool[TOOL_CHANGE_TRAFFIC_LIGHT], welt->get_active_player() );
    for(uint8 i=0; i<4; i++){
      const koord3d neighbour = k + koord3d(koord::nesw[i],0);
      grund_t* gr = welt->lookup(neighbour);
      const roadsign_t* rs = gr->find<roadsign_t>();
      if( rs && rs->get_desc()->is_traffic_light() && welt->get_active_player() == rs->get_owner() ){
	if( rs->get_ticks_amber_ow() != (uint8)v.i ){
	  set_tool_at(comp, v, neighbour);
	}
      }
    }
  }
}

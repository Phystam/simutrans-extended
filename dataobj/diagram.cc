#include "diagram.h"

#include "../tpl/vector_tpl.h"
#include "../simline.h"
#include "../simhalt.h"
#include "../dataobj/schedule.h"

diagram_t::diagram_t(){
	//initialized by empty value at the beginning of the list
	// station_name_list.append();
	// is_reverse_list.append();
	// total_distance_list.append();
	// is_reverse_line=false;
}

diagram_t::~diagram_t(){
}



void diagram_t::append_diagram_entry(char* station_name, int total_distance, bool is_reverse){
	station_name_list.append(station_name);
	total_distance_list.append(total_distance);
	is_reverse_list.append(is_reverse);

}

void diagram_t::set_diagram_entries_from(linehandle_t line){
	std::cout<<"called update_diagram()"<<std::endl;
	schedule_t* schedule;
	if(line.is_bound()){
		schedule = line->get_schedule();
	}else{
		return;
	}

	const minivec_tpl<schedule_entry_t>& entries = schedule->entries;
	const uint8 size = entries.get_count();
	if (size <= 1) return; //nothing to do
	std::cout <<"size="<<(int)size<<std::endl;
	
	bool mirrored =schedule->is_mirrored();

	//acquire station name list
	for (sint16 i = 0; i < size - 1; i++) {
		std::cout <<"ok "<<(int)i<<std::endl;
		const halthandle_t halt = haltestelle_t::get_halt(entries[i].pos, line->get_owner());
		const char *halt_name = halt.is_bound() ? new char[strlen(halt->get_name()) + 1] : new char[128]; 
		halt_name = halt->get_name();
		std::cout << halt_name << std::endl;
		uint32 station_size=station_name_list.get_size();
		bool contained=false;
		if( station_name_list.is_contained(halt_name) ){
			contained=true;
		}
		if(!contained){
			station_name_list.append(halt_name);
			total_distance_list.append(1000*i);
		}
	}
	uint32 station_size=station_name_list.get_size();
	
	for(int i=0;i<station_size - 1;i++){
		std::cout<<i<<"th station name is "<<station_name_list[i]<<std::endl;
	}
	
	
	
	if (mirrored) {
		std::cout<<"bidirectional_order"<<std::endl;
		sint16 first_halt_index = -1;
		sint16 last_halt_index = -1;
	}

}

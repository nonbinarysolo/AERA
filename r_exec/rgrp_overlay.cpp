//	rgrp_overlay.cpp
//
//	Author: Eric Nivel
//
//	BSD license:
//	Copyright (c) 2010, Eric Nivel
//	All rights reserved.
//	Redistribution and use in source and binary forms, with or without
//	modification, are permitted provided that the following conditions are met:
//
//   - Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//   - Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
//   - Neither the name of Eric Nivel nor the
//     names of their contributors may be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
//	THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND ANY
//	EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//	DISCLAIMED. IN NO EVENT SHALL THE REGENTS AND CONTRIBUTORS BE LIABLE FOR ANY
//	DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include	"rgrp_overlay.h"
#include	"rgrp_controller.h"
#include	"pgm_controller.h"
#include	"mem.h"


namespace	r_exec{

	BindingOverlay::BindingOverlay(__Controller	*c,RGroup	*group,BindingMap	*bindings,uint8	reduction_mode):RGRPOverlay(c),reduction_mode(reduction_mode){

		this->bindings.init(group);	//	init bindings from r-group.

		if(bindings)	//	bind some of the variables in use in the r-group to values already assigned.
			this->bindings.add(bindings);

		UNORDERED_MAP<uint32,P<View> >::const_iterator	v;
		for(v=group->ipgm_views.begin();v!=group->ipgm_views.end();++v)	//	start will all binders.
			binders.push_back(v->second);

		birth_time=Now();
	}

	BindingOverlay::BindingOverlay(BindingOverlay	*original):RGRPOverlay(original->controller),reduction_mode(original->reduction_mode){

		//	copy binders from the original.
		std::list<P<View> >::const_iterator	b;
		for(b=original->binders.begin();b!=original->binders.end();++b)
			binders.push_back(*b);

		//	copy bindings from the original.
		bindings.init(&original->bindings);

		//	reintegrate the last bound variables (i.e. un-bind them).
		for(uint16	i=0;i<original->last_bound_variable_objects.size();++i)
			bindings.unbind_object(original->last_bound_variable_objects[i]);
		for(uint16	i=0;i<original->last_bound_code_variables.size();++i){

			Atom	a=original->last_bound_code_variables[i];
			if(a.getDescriptor()==Atom::NUMERICAL_VARIABLE)
				bindings.unbind_atom(a);
			else
				bindings.unbind_structure(a);
		}

		birth_time=Now();
	}

	BindingOverlay::~BindingOverlay(){
	}

	void	BindingOverlay::bind(Code	*value_source,uint16	value_index,Code	*variable_source,uint16	variable_index){	//	called back by _subst.

		if(discard_bindings)
			return;

		Atom	var=variable_source->code(variable_index);	//	variable_source is an ipgm (binder); variable_index points to the variable defined in its tpl args.
		float32	multiplier=var.getMultiplier();
		switch(var.getDescriptor()){
		case	Atom::NUMERICAL_VARIABLE:{
			
			Atom	val=value_source->code(value_index);
			Atom	v=bindings.atoms[var];
			if(!!v){

				float32	min=v.asFloat()*(1-multiplier);
				float32	max=v.asFloat()*(1+multiplier);

				if(val.asFloat()<min	||	val.asFloat()>max)
					discard_bindings=true;	//	at least one value differs from an existing binding.
				else
					no_binding_needed=true;
				return;
			}

			bindings.bind_atom(var,val);
			last_bound_code_variables.push_back(var);
			break;
		}case	Atom::STRUCTURAL_VARIABLE:{	//	value_index points to the head of an embedded structure.
			
			std::vector<Atom>	val;
			std::vector<Atom>	v=bindings.structures[var];
			if(v.size()>0){

				switch(v[0].getDescriptor()){
				case	Atom::TIMESTAMP:{	//	tolerance is used.

					uint64	vt=Utils::GetTimestamp(&v[0]);
					uint64	now=Now();
					uint64	delta_t;
					if(now>vt)
						delta_t=now-vt;
					else
						delta_t=vt-now;
					uint64	min=delta_t*(1-multiplier);
					uint64	max=delta_t*(1+multiplier);
					uint64	t=Utils::GetTimestamp(&value_source->code(value_index));
					if(t<now)
						t=now-t;
					else
						t=t-now;
					if(t<min	||	t>max)
						discard_bindings=true;	//	the value differs from an existing binding.
					else
						no_binding_needed=true;
					return;
				}case	Atom::STRING:	//	tolerance is not used.
					for(uint16	i=0;i<=value_source->code(value_index).getAtomCount();++i){

						if(value_source->code(value_index+i)!=v[i]){

							discard_bindings=true;	//	at least part of value differs from an existing binding.
							return;
						}
					}
					no_binding_needed=true;
					return;
				case	Atom::OBJECT:	//	tolerance is used to compare numerical structure members.
					for(uint16	i=0;i<=value_source->code(value_index).getAtomCount();++i){

						if(value_source->code(value_index+i).isFloat()){

							float32	min=v[i].asFloat()*(1-multiplier);
							float32	max=v[i].asFloat()*(1+multiplier);
							float	v=value_source->code(value_index+i).asFloat();
							if(v<min	||	v>max){

								discard_bindings=true;	//	at least one value differs from an existing binding.
								return;
							}
						}else	if(value_source->code(value_index+i)!=v[i]){

							discard_bindings=true;	//	at least one value differs from an existing binding.
							return;
						}
					}
					no_binding_needed=true;
					return;
				}
			}

			for(uint16	i=0;i<=value_source->code(value_index).getAtomCount();++i)
				val.push_back(value_source->code(value_index+i));
			bindings.bind_structure(var,val);
			last_bound_code_variables.push_back(var);
			break;
		}case	Atom::R_PTR:{

			Code	*variable_object=variable_source->get_reference(var.asIndex());
			Code	*v=bindings.objects[variable_object];
			if(v){
			
				if(v!=value_source)
					discard_bindings=true;	//	at least one value differs from an existing binding.
				else
					no_binding_needed=true;
				return;
			}

			bindings.bind_object(variable_object,value_source);
			last_bound_variable_objects.push_back(variable_object);
			break;
		}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	FwdOverlay::FwdOverlay(__Controller	*c,RGroup	*group,BindingMap	*bindings,uint8	reduction_mode):BindingOverlay(c,group,bindings,reduction_mode){
	}

	FwdOverlay::FwdOverlay(FwdOverlay	*original):BindingOverlay(original){
	}

	FwdOverlay::~FwdOverlay(){
	}

	void	FwdOverlay::reduce(r_exec::View	*input){

		//	for all remaining binders in this overlay:
		//		binder_view->take_input(input,this); 'this' is needed for calling the overlay back from _subst.
		//		_subst: callback (bind()):
		//			store values for variables for this overlay.
		//	if at least one binder matched:
		//		create an overlay (binders and bindings as they were prior to the match).
		//		remove the binder from this overlay.
		//	if no more unbound values, fire and kill the overlay.

		reductionCS.enter();
		if(alive){

			last_bound_variable_objects.clear();
			last_bound_code_variables.clear();
			discard_bindings=false;
			no_binding_needed=false;

			std::list<P<View> >::const_iterator	b;
			for(b=binders.begin();b!=binders.end();++b){	//	binders called sequentially (at most one can bind an input).

				((PGMController	*)(*b)->controller)->take_input(input,this);
				if(discard_bindings)
					break;
				if(no_binding_needed	||	last_bound_variable_objects.size()	||	last_bound_code_variables.size()){	//	at least one variable was bound.

					FwdOverlay	*offspring=new	FwdOverlay(this);
					controller->add(offspring);

					if(input->object->get_sim())
						reduction_mode|=RDX_MODE_SIMULATION;
					if(input->object->get_asmp())
						reduction_mode|=RDX_MODE_ASSUMPTION;

					if(!bindings.unbound_var_count){

						kill();
						controller->remove(this);
						((FwdController	*)controller)->fire(&bindings,reduction_mode);
					}else
						binders.erase(b);
					break;
				}
			}
		}
		reductionCS.leave();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	InvOverlay::InvOverlay(InvController	*c):RGRPOverlay(c),success(false){
	}

	InvOverlay::~InvOverlay(){
	}

	void	InvOverlay::bind(Code	*value_source,uint16	value_index,Code	*variable_source,uint16	variable_index){	//	the object being bound may contain unbound variables.

		Atom	var=variable_source->code(variable_index);	//	variable_source is an ipgm (binder); variable_index points to the variable defined in its tpl args.
		switch(var.getDescriptor()){
		case	Atom::NUMERICAL_VARIABLE:{

			Atom	val=value_source->code(value_index);
			bindings.atoms[var]=val;
			break;
		}case	Atom::STRUCTURAL_VARIABLE:{	//	value_index points to the head of an embedded structure.
			
			std::vector<Atom>	val;
			for(uint16	i=0;i<=value_source->code(value_index).getAtomCount();++i)
				val.push_back(value_source->code(value_index+i));
			bindings.structures[var]=val;
			break;
		}case	Atom::R_PTR:{

			Code	*variable_object=variable_source->get_reference(var.asIndex());
			bindings.objects[variable_object]=value_source;
			break;
		}
		}

		success=true;
	}
}
/*
 ==============================================================================
 
 This file is part of the UGEN++ library
 Copyright 2008-10 by Martin Robinson www.miajo.co.uk
 
 ------------------------------------------------------------------------------
 
 UGEN++ can be redistributed and/or modified under the terms of the
 GNU General Public License, as published by the Free Software Foundation;
 either version 2 of the License, or (at your option) any later version.
 
 UGEN++ is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with UGEN++; if not, visit www.gnu.org/licenses or write to the
 Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 Boston, MA 02111-1307 USA
 
 The idea for this project and code in the UGen implementations is
 derived from SuperCollider which is also released under the 
 GNU General Public License:
 
 SuperCollider real time audio synthesis system
 Copyright (c) 2002 James McCartney. All rights reserved.
 http://www.audiosynth.com
 
 ==============================================================================
 */

#ifndef _UGEN_ugen_UISlider_H_
#define _UGEN_ugen_UISlider_H_


#include "../basics/ugen_ScalarUGens.h"

/** @ingroup UGenInternals */
class UISliderUGenInternal : public FloatPtrUGenInternal
{
public:
	UISliderUGenInternal(UISlider* slider) throw();
	~UISliderUGenInternal() throw();
	
	void setValue(float newValue);
	
private:
	float value;
	UISlider* slider_;
	void* voidPeer;
};

/** A UGen which takes its value source from a UISlider (iPhone).
 @ingroup GUITools ControlUGens */
class UISliderUGen : public UGen 
{
public:
	UISliderUGen(UISlider* slider) throw();
};




#endif // _UGEN_ugen_UISlider_H_

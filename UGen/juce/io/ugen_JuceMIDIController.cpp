// $Id$
// $HeadURL$

/*
 ==============================================================================
 
 This file is part of the UGEN++ library
 Copyright 2008-11 The University of the West of England.
 by Martin Robinson
 
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

#ifndef UGEN_NOEXTGPL

#include "../../core/ugen_StandardHeader.h"

#ifdef JUCE_VERSION

BEGIN_UGEN_NAMESPACE

#include "ugen_JuceMIDIController.h"

MIDIControllerInternal::MIDIControllerInternal(const int midiChannel, const int controllerNumber,
											   const float minVal, const float maxVal,
											   const ExternalControlSource::Warp warp, void* port) throw()
:	ExternalControlSourceInternal(minVal, maxVal, warp),
	midiChannel_(midiChannel),
	controllerNumber_(controllerNumber),
	port_(port)
{
}

void MIDIControllerInternal::handleIncomingMidiMessage (MidiInput* source, const MidiMessage& message) throw()
{	
	if(port_ != 0 && port_ != source)						return;
	if(message.isForChannel(midiChannel_) == false)			return;
	if(message.isController() == false)						return;
	if(message.getControllerNumber() != controllerNumber_)	return;
	
	setNormalisedValue(message.getControllerValue() / 127.f);
}

MIDIController::MIDIController(const int midiChannel, const int controllerNumber,
							   const float minVal, const float maxVal, 
							   const ExternalControlSource::Warp warp, 
							   void* port) throw()
{
	internal = new MIDIControllerInternal(midiChannel, controllerNumber, minVal, maxVal, warp, port);
}


END_UGEN_NAMESPACE

#endif

#endif

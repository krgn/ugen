// $Id:$
// $HeadURL:$

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

#include "../core/ugen_StandardHeader.h"

BEGIN_UGEN_NAMESPACE

#include "ugen_Trig.h"

TrigUGenInternal::TrigUGenInternal(UGen const& input) throw()
:	UGenInternal(NumInputs)
{
	inputs[Input] = input;
	lastTrig = 0.f;
}

void TrigUGenInternal::processBlock(bool& shouldDelete, const unsigned int blockID, const int channel) throw()
{
	int numSamplesToProcess = uGenOutput.getBlockSize();
	float* outputSamples = uGenOutput.getSampleData();
	float* inputSamples = inputs[Input].processBlock(shouldDelete, blockID, channel);
	
	LOCAL_DECLARE(float, lastTrig);
	
	while(numSamplesToProcess--)
	{
		float thisTrig = *inputSamples++;
		
		if(thisTrig > 0.f && lastTrig <= 0.f)
			*outputSamples++ = 1.f;
		else
			*outputSamples++ = 0.f;
		
		lastTrig = thisTrig;
	}
	
	LOCAL_COPY(lastTrig);
}

Trig::Trig(UGen const& input) throw()
{
	initInternal(input.getNumChannels());
	
	for(int i = 0; i < numInternalUGens; i++)
	{
		internalUGens[i] = new TrigUGenInternal(input);
	}
}

END_UGEN_NAMESPACE
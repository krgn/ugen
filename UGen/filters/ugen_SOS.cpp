// $Id: ugen_SOS.cpp 979 2010-01-09 15:47:39Z mgrobins $
// $HeadURL: http://164.11.131.73/svn/CEMS/mt/other/UGenProject/UGen/filters/ugen_SOS.cpp $

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

#include "ugen_SOS.h"
#include "../core/ugen_Constants.h"
#include "../basics/ugen_InlineUnaryOps.h"
#include "../basics/ugen_InlineBinaryOps.h"



SOSUGenInternal::SOSUGenInternal(SOS_InputsWithTypesOnly) throw()
:	UGenInternal(NumInputs)
{	
	UGen inputArgs[] = { SOS_InputsNoTypes };
	for(int i = 0; i < NumInputs; i++)
	{
		inputs[i] = inputArgs[i];
	}
}

UGenInternal* SOSUGenInternal::getChannel(const int channel) throw()
{
	return new SOSUGenInternal(inputs[Input].getChannel(channel),
							   inputs[A0].getChannel(channel),
							   inputs[A1].getChannel(channel),
							   inputs[A2].getChannel(channel),
							   inputs[B1].getChannel(channel),
							   inputs[B2].getChannel(channel));
}

void SOSUGenInternal::processBlock(bool& shouldDelete, const unsigned int blockID, const int channel) throw()
{
	int filterLoops = UGen::getFilterLoops();
	int filterRemain = UGen::getFilterRemain();
	float* outputSamples = uGenOutput.getSampleData();
	float* inputSamples = inputs[Input].processBlock(shouldDelete, blockID, channel);
	float* a0Samples = inputs[A0].processBlock(shouldDelete, blockID, channel);
	float* a1Samples = inputs[A1].processBlock(shouldDelete, blockID, channel);
	float* a2Samples = inputs[A2].processBlock(shouldDelete, blockID, channel);
	float* b1Samples = inputs[B1].processBlock(shouldDelete, blockID, channel);
	float* b2Samples = inputs[B2].processBlock(shouldDelete, blockID, channel);
	float y0;
	
	while(filterLoops--)
	{
		y0 = *inputSamples++ + *b1Samples++ * y1 + *b2Samples++ * y2; 
		*outputSamples++ = *a0Samples++ * y0 + *a1Samples++ * y1 + *a2Samples++ * y2;
		
		y2 = *inputSamples++ + *b1Samples++ * y0 + *b2Samples++ * y1; 
		*outputSamples++ = *a0Samples++ * y2 + *a1Samples++ * y0 + *a2Samples++ * y1;
		
		y1 = *inputSamples++ + *b1Samples++ * y2 + *b2Samples++ * y0; 
		*outputSamples++ = *a0Samples++ * y1 + *a1Samples++ * y2 + *a2Samples++ * y0;
	}
	
	while(filterRemain--)
	{
		y0 = *inputSamples++ + *b1Samples++ * y1 + *b2Samples++ * y2; 
		*outputSamples++ = *a0Samples++ * y0 + *a1Samples++ * y1 + *a2Samples++ * y2;

		y2 = y1; 
		y1 = y0;
	}
	
	y1 = zap(y1);
	y2 = zap(y2);
}

void SOSUGenInternal::initValue(const float value) throw()
{
	float checkedValue = zap(value);
	UGenInternal::initValue(checkedValue);
	y1 = y2 = checkedValue;
}

SOS::SOS(SOS_InputsWithTypesOnly) throw()
{
	UGen inputs[] = { SOS_InputsNoTypes };
	const int numInputChannels = findMaxInputChannels(numElementsInArray(inputs), inputs);		
	initInternal(numInputChannels);
	
	for(int i = 0; i < numInternalUGens; i++)
	{
		internalUGens[i] = new SOSUGenInternal(SOS_InputsNoTypes);
		internalUGens[i]->initValue(input.getValue(i) * a0.getValue(i)); // a0 is gain
	}
}

END_UGEN_NAMESPACE

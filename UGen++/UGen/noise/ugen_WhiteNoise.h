// $Id: ugen_WhiteNoise.h 979 2010-01-09 15:47:39Z mgrobins $
// $HeadURL: http://dsrowlan@164.11.131.73/svn/CEMS/mt/other/UGenProject/UGen/noise/ugen_WhiteNoise.h $

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

#ifndef _UGEN_ugen_WhiteNoise_H_
#define _UGEN_ugen_WhiteNoise_H_

#include "../core/ugen_UGen.h"
#include "../basics/ugen_MulAdd.h"
#include "../core/ugen_Random.h"

/** @ingroup UGenInternals */
class WhiteNoiseUGenInternal : public UGenInternal
{
public:
	WhiteNoiseUGenInternal() throw();
	void processBlock(bool& shouldDelete, const unsigned int blockID, const int channel) throw();
	
	enum Inputs { NoInputs };
	
protected:
	Ran088 random;
};

/** White noise generator.
 Generates noise whose spectrum has equal power at all frequencies.
 @see PinkNoise, BrownNoise
 @ingroup AllUGens NoiseUGens */
ExpandingMulAddUGenDeclaration(WhiteNoise, (), (MulAdd_ArgsCall), (), (MulAdd_ArgsDeclare), COMMON_UGEN_DOCS MulAddArgs_Docs);

#if defined(UGEN_USER_MODE) && defined(UGEN_SCSTYLE)
#define WhiteNoise WhiteNoise()
#endif


#endif // _UGEN_ugen_WhiteNoise_H_

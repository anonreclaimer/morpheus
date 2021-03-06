# [Morpheus](https://github.com/raynorpat/morpheus)
Morpheus is an idTech 4 Engine Modification.

Some of the major features currently implemented are:
 * Much improved premake based build system
 * x86_64 binary support
 * GLSL renderer backend
 * OpenAL sound system improvements (you can use OpenAL Soft for example and uses EFX for environmental audio)

## GENERAL NOTES

### Game data and patching:

Morpheus does not contain any Doom 3 game data, the game data is still
covered by the original EULA and must be obeyed as usual.
We do not support using the Doom 3 demo data.

You must patch the game to the latest version. ( 1.3.1 )

NOTE: Doom 3 and Doom 3: Resurrection of Evil are available from the Steam store:
 * Doom 3 - <http://store.steampowered.com/app/9050/>
 * Doom 3: Resurrection of Evil - <http://store.steampowered.com/app/9070/>

### Compiling:

The build system is based on PreMake: http://PreMake.org/

For Windows users with MSVC 2012 you'd simply generate the solution files using

`premake5 vs2012`

OSX users would need use

`premake5 xcode4`

*nix users would probably use

`premake5 gmake`

NOTE: Currently the only working backend is the Win32 one. If you'd like to help write a port to SDL2 for OS X and *nix feel free to fork and submit a patch.

### MayaImport:

The code for our Maya export plugin is included, if you are a Maya licensee
you can obtain the SDK from Autodesk.



## LICENSES

See COPYING.txt for the GNU GENERAL PUBLIC LICENSE

ADDITIONAL TERMS:
```
The Doom 3 GPL Source Code is also subject to certain additional terms.
You should have received a copy of these additional terms immediately following the terms and conditions of the GNU GPL which accompanied the Doom 3 Source Code.
If not, please request a copy in writing from id Software at id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.`
```

EXCLUDED CODE:
```
The code described below and contained in the Doom 3 GPL Source Code release is not part of the Program covered by the GPL and is expressly excluded from its terms.
You are solely responsible for obtaining from the copyright holder a license for such code and complying with the applicable license terms.`
```

### Curl library
neo/curl/*
		
	COPYRIGHT AND PERMISSION NOTICE

	Copyright (c) 1996 - 2004, Daniel Stenberg, <daniel@haxx.se>.

	All rights reserved.

	Permission to use, copy, modify, and distribute this software for any purpose
	with or without fee is hereby granted, provided that the above copyright
	notice and this permission notice appear in all copies.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF THIRD PARTY RIGHTS. IN
	NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
	DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
	OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
	OR OTHER DEALINGS IN THE SOFTWARE.

	Except as contained in this notice, the name of a copyright holder shall not
	be used in advertising or otherwise to promote the sale, use or other dealings
	in this Software without prior written authorization of the copyright holder.

### JPEG library
neo/renderer/jpeg-6/*

	Copyright (C) 1991-1995, Thomas G. Lane

	Permission is hereby granted to use, copy, modify, and distribute this
	software (or portions thereof) for any purpose, without fee, subject to these
	conditions:
	(1) If any part of the source code for this software is distributed, then this
	README file must be included, with this copyright and no-warranty notice
	unaltered; and any additions, deletions, or changes to the original files
	must be clearly indicated in accompanying documentation.
	(2) If only executable code is distributed, then the accompanying
	documentation must state that "this software is based in part on the work of
	the Independent JPEG Group".
	(3) Permission for use of this software is granted only if the user accepts
	full responsibility for any undesirable consequences; the authors accept
	NO LIABILITY for damages of any kind.

	These conditions apply to any software derived from or based on the IJG code,
	not just to the unmodified library.  If you use our work, you ought to
	acknowledge us.

	NOTE: unfortunately the README that came with our copy of the library has
	been lost, so the one from release 6b is included instead. There are a few
	'glue type' modifications to the library to make it easier to use from
	the engine, but otherwise the dependency can be easily cleaned up to a
	better release of the library.

### OggVorbis 
neo/sound/OggVorbis/*
			
	Copyright (c) 2002, Xiph.org Foundation

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	- Redistributions of source code must retain the above copyright
	notice, this list of conditions and the following disclaimer.

	- Redistributions in binary form must reproduce the above copyright
	notice, this list of conditions and the following disclaimer in the
	documentation and/or other materials provided with the distribution.

	- Neither the name of the Xiph.org Foundation nor the names of its
	contributors may be used to endorse or promote products derived from
	this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION
	OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
	LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

### PropTree
neo/tools/common/PropTree/*

	Copyright (C) 1998-2001 Scott Ramsay
	sramsay@gonavi.com
	http://www.gonavi.com

	This material is provided "as is", with absolutely no warranty expressed
	or implied. Any use is at your own risk.
	 
	Permission to use or copy this software for any purpose is hereby granted 
	without fee, provided the above notices are retained on all copies.
	Permission to modify the code and to distribute modified code is granted,
	provided the above notices are retained, and a notice that the code was
	modified is included with the above copyright notice.
	 
	If you use this code, drop me an email.  I'd like to know if you find the code
	useful.

### OpenAL SDK
neo/sys/openal/*

	/**
	 * OpenAL cross platform audio library
	 * Copyright (C) 2008 by authors.
	 * This library is free software; you can redistribute it and/or
	 *  modify it under the terms of the GNU Library General Public
	 *  License as published by the Free Software Foundation; either
	 *  version 2 of the License, or (at your option) any later version.
	 *
	 * This library is distributed in the hope that it will be useful,
	 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
	 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	 *  Library General Public License for more details.
	 *
	 * You should have received a copy of the GNU Library General Public
	 *  License along with this library; if not, write to the
	 *  Free Software Foundation, Inc., 59 Temple Place - Suite 330,
	 *  Boston, MA  02111-1307, USA.
	 * Or go to http://www.gnu.org/copyleft/lgpl.html
	 */

### Base64 implementation
neo/idlib/Base64.cpp

	Copyright (c) 1996 Lars Wirzenius.  All rights reserved.

	June 14 2003: TTimo <ttimo@idsoftware.com>
		modified + endian bug fixes
		http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=197039

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	1. Redistributions of source code must retain the above copyright
	   notice, this list of conditions and the following disclaimer.

	2. Redistributions in binary form must reproduce the above copyright
	   notice, this list of conditions and the following disclaimer in the
	   documentation and/or other materials provided with the distribution.

	THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
	IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
	INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
	SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
	HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
	STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
	ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
	POSSIBILITY OF SUCH DAMAGE.

### IO on .zip files using portions of zlib
src/framework/Unzip.cpp

	Copyright (C) 1998 Gilles Vollant
	zlib is Copyright (C) 1995-1998 Jean-loup Gailly and Mark Adler

	  This software is provided 'as-is', without any express or implied
	  warranty.  In no event will the authors be held liable for any damages
	  arising from the use of this software.

	  Permission is granted to anyone to use this software for any purpose,
	  including commercial applications, and to alter it and redistribute it
	  freely, subject to the following restrictions:

	  1. The origin of this software must not be misrepresented; you must not
		 claim that you wrote the original software. If you use this software
		 in a product, an acknowledgment in the product documentation would be
		 appreciated but is not required.
	  2. Altered source versions must be plainly marked as such, and must not be
		 misrepresented as being the original software.
	  3. This notice may not be removed or altered from any source distribution.

### MD4 Message-Digest Algorithm
neo/idlib/hashing/MD4.cpp

	Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
	rights reserved.

	License to copy and use this software is granted provided that it
	is identified as the "RSA Data Security, Inc. MD4 Message-Digest
	Algorithm" in all material mentioning or referencing this software
	or this function.

	License is also granted to make and use derivative works provided
	that such works are identified as "derived from the RSA Data
	Security, Inc. MD4 Message-Digest Algorithm" in all material
	mentioning or referencing the derived work.

	RSA Data Security, Inc. makes no representations concerning either
	the merchantability of this software or the suitability of this
	software for any particular purpose. It is provided "as is"
	without express or implied warranty of any kind.

	These notices must be retained in any copies of any part of this
	documentation and/or software.

### MD5 Message-Digest Algorithm
neo/idlib/hashing/MD5.cpp

	This code implements the MD5 message-digest algorithm.
	The algorithm is due to Ron Rivest.  This code was
	written by Colin Plumb in 1993, no copyright is claimed.
	This code is in the public domain; do with it what you wish.

### CRC32 Checksum
neo/idlib/hashing/CRC32.cpp

	Copyright (C) 1995-1998 Mark Adler

### OpenGL headers
neo/renderer/glext.h & neo/renderer/wglext.h

	/*
	** License Applicability. Except to the extent portions of this file are
	** made subject to an alternative license as permitted in the SGI Free
	** Software License B, Version 1.1 (the "License"), the contents of this
	** file are subject only to the provisions of the License. You may not use
	** this file except in compliance with the License. You may obtain a copy
	** of the License at Silicon Graphics, Inc., attn: Legal Services, 1600
	** Amphitheatre Parkway, Mountain View, CA 94043-1351, or at:
	** 
	** http://oss.sgi.com/projects/FreeB
	** 
	** Note that, as provided in the License, the Software is distributed on an
	** "AS IS" basis, with ALL EXPRESS AND IMPLIED WARRANTIES AND CONDITIONS
	** DISCLAIMED, INCLUDING, WITHOUT LIMITATION, ANY IMPLIED WARRANTIES AND
	** CONDITIONS OF MERCHANTABILITY, SATISFACTORY QUALITY, FITNESS FOR A
	** PARTICULAR PURPOSE, AND NON-INFRINGEMENT.
	** 
	** Original Code. The Original Code is: OpenGL Sample Implementation,
	** Version 1.2.1, released January 26, 2000, developed by Silicon Graphics,
	** Inc. The Original Code is Copyright (c) 1991-2002 Silicon Graphics, Inc.
	** Copyright in any portions created by third parties is as indicated
	** elsewhere herein. All Rights Reserved.
	** 
	** Additional Notice Provisions: This software was created using the
	** OpenGL(R) version 1.2.1 Sample Implementation published by SGI, but has
	** not been independently verified as being compliant with the OpenGL(R)
	** version 1.2.1 Specification.
	*/

### NV-CONTROL X Extension
neo/sys/linux/libXNVCtrl/*

	Copyright NVIDIA Corporation

### ExtUtil.h
neo/sys/linux/extutil.h

	/*
	 * $Xorg: extutil.h,v 1.4 2001/02/09 02:03:24 xorgcvs Exp $
	 *
	Copyright 1989, 1998  The Open Group

	Permission to use, copy, modify, distribute, and sell this software and its
	documentation for any purpose is hereby granted without fee, provided that
	the above copyright notice appear in all copies and that both that
	copyright notice and this permission notice appear in supporting
	documentation.

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
	OPEN GROUP BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
	AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
	CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

	Except as contained in this notice, the name of The Open Group shall not be
	used in advertising or otherwise to promote the sale, use or other dealings
	in this Software without prior written authorization from The Open Group.
	 *
	 * Author:  Jim Fulton, MIT The Open Group
	 * 
	 *                     Xlib Extension-Writing Utilities
	 *
	 * This package contains utilities for writing the client API for various
	 * protocol extensions.  THESE INTERFACES ARE NOT PART OF THE X STANDARD AND
	 * ARE SUBJECT TO CHANGE!
	 */


## Credits

### Maintainers

  * Pat 'raynorpat' Raynor <raynorpat@gmail.com>

### Significant contributions from

  * Doom 3 BFG Edition GPL Source <https://github.com/id-Software/DOOM-3-BFG>
  * MH's OpenGL ARB Buffer Range patch <http://pastebin.com/rHrwP0nA>
  
  
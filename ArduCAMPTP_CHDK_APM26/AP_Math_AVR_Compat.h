// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

#ifndef __AP_MATH_AVR_COMPAT_H
#define __AP_MATH_AVR_COMPAT_H

/*
ArduCam PTP CHDK - Arduino based Remote CHDK Camera Control.
Copyright (c) 2011.  All rights reserved.
This firmware is part of the ArduCAM OSD Project: an open source Arduino based OSD and Camera Control.

Author(s): Sandro Benigno
                  Legacy PTP commands from Canon's PS-ReCSDK 1.1.0e
                  Custom extended PTP commands from CHDK's PTP implementation
                  USB host and PTP library from Oleg Mazurov - circuitsathome.com
                  
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

// This file defines the floating-point version of standard C math
// functions on doubles, if they are not present in avr-libc.

#ifndef cosf
# define cosf cos
#endif

#ifndef sinf
# define sinf sin
#endif

#ifndef tanf
# define tanf tan
#endif

#ifndef fabsf
# define fabsf fabs
#endif

#ifndef fmodf
# define fmodf fmod
#endif

#ifndef sqrtf
# define sqrtf sqrt
#endif

#ifndef cbrtf
# define cbrtf cbrt
#endif

#ifndef hypotf
# define hypotf hypot
#endif

#ifndef squaref
# define squaref square
#endif

#ifndef floorf
# define floorf floor
#endif

#ifndef ceilf
# define ceilf ceil
#endif

#ifndef frexpf
# define frexpf frexp
#endif

#ifndef ldexpf
# define ldexpf ldexp
#endif

#ifndef expf
# define expf exp
#endif

#ifndef coshf
# define coshf cosh
#endif

#ifndef sinhf
# define sinhf sinh
#endif

#ifndef tanhf
# define tanhf tanh
#endif

#ifndef acosf
# define acosf acos
#endif

#ifndef asinf
# define asinf asin
#endif

#ifndef atanf
# define atanf atan
#endif

#ifndef atan2f
# define atan2f atan2
#endif

#ifndef logf
# define logf log
#endif

#ifndef log10f
# define log10f log10
#endif

#ifndef powf
# define powf pow
#endif

#ifndef isnanf
# define isnanf isnan
#endif

#ifndef isinff
# define isinff isinf
#endif

#ifndef isfinitef
# define isfinitef isfinite
#endif

#ifndef copysignf
# define copysignf copysign
#endif

#ifndef signbitf
# define signbitf signbit
#endif

#ifndef fdimf
# define fdimf fdim
#endif

#ifndef fmaf
# define fmaf fma
#endif

#ifndef fminf
# define fminf fmin
#endif

#ifndef truncf
# define truncf trunc
#endif

#ifndef roundf
# define roundf round
#endif

#ifndef lroundf
# define lroundf lround
#endif

#ifndef lrintf
# define lrintf lrint
#endif

#endif  // !defined AP_MATH_AVR_COMPAT_H


3dr-camera-ctrl
===============

3DR Camera Control Board with USB Host

ArduCam PTP CHDK - Arduino based Remote CHDK Camera Control.
Copyright (c) 2011.  All rights reserved.
This firmware is part of the ArduCAM OSD Project: an open source Arduino based OSD and Camera Control.

<pre>
Author(s): Sandro Benigno
           Legacy PTP commands from Canon's PS-ReCSDK 1.1.0e
           Custom extended PTP commands from CHDK's PTP implementation
           USB host and PTP library from Oleg Mazurov - circuitsathome.com

Note: The PTP library used here was modified by the author.
      There are special characteristics needed to cover the scripting approach.
      The CHDK PTP Transaction has a wider RX buffer (reflected on the dataSize flag).
</pre>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.  You should have received a copy of the GNU General Public License along with this program. If not, see <http://www.gnu.org/licenses/>.

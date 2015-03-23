GeoShooting base-code for Camera Control over CHDK PTP
This code is part of ArduCAMPTP_CHDK's implementetion for 3DRobotics.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2+
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

Project page: https://github.com/sandrobenigno/3dr-camera-ctrl

This script receives arguments in a packet of PTP Transaction:

lat   = params[1] <- Latitude.     (E.g. 47.266273)
lon   = params[2] <- Longitude.    (E.g. 11.302096)
alt   = params[3] <- Altitude ASL. (E.g. 1083.22)
pitch = params[4] <- Pitch         (-180 to 180)
row   = params[5] <- Roll          (-180 to 180)
yaw   = params[6] <- Heading       (0.00 <= X < 360.00)

It takes a picture and create/update a file "geostore.csv" on the folder session the taken picture.

Each line of this file looks like this:
IMG_0673.JPG,47.266273,11.302096,1083.22,7,-35,210.00 
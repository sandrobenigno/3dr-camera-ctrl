GeoShooting base-code for Camera Control over CHDK PTP
This code is part of ArduCAMPTP_CHDK's implementetion for 3DRobotics.

Licenced by Sandro Benigno under CC 3.0 BY-SA
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
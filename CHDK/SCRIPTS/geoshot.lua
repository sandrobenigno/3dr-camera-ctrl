--[[

GeoShooting base-code for Camera Control over CHDK PTP
The current script is part of ArduCAMPTP_CHDK's implementetion for 3DRobotics.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2+
of the License, or (at your option) any later version.
Project page: https://github.com/sandrobenigno/3dr-camera-ctrl

PTP Transaction Data:
[lat][lon][alt_asl][pitch][roll][heading]
File saved: "geostore.csv" (session folder of taken pictures)
Per line data: IMG_0673.JPG,47.266273,11.302096,1083.22,7,-35,210.00 

@title GeoShot 1.0

]]


-- Functions --------------------------------------------------------------

function getImgName()
	local ImgName = nil
	ImgName=string.format("IMG_%04d.JPG",get_exp_count())
	return ImgName
end

function writeGeoData(path, data)
  local F
	F = io.open(path, "a+")
	F:write(data.."\r\n")
	F:close()
end


-- Main Code --------------------------------------------------------------

cls()

params = {...}

shoot()
repeat sleep(50)  until get_shooting() == false

local imgname = nil
local dirpath = nil
imgname = getImgName()
dirpath = get_image_dir()

geodata = (imgname..","..params[1]..","..params[2]..","..params[3]..","..params[4]..","..params[5]..","..params[6])
writeGeoData(dirpath.."/geostore.csv", geodata)

cls()
@echo off
REM Requires openscad folder in path.

@echo Building 28mm
REM 28mm depth (old style switch) - easy fit
"C:\Program Files\OpenSCAD\openscad.com" -o Cover-28mm-V1.1.stl -D depth=28;showCover=true;showBase=false;showPcb=false;showSwitch=false;minimalCover=false;pcbVersion=1 JediLightSwitch.scad
"C:\Program Files\OpenSCAD\openscad.com" -o Cover-28mm-V1.2.stl -D depth=28;showCover=true;showBase=false;showPcb=false;showSwitch=false;minimalCover=false;pcbVersion=2 JediLightSwitch.scad
"C:\Program Files\OpenSCAD\openscad.com" -o Cover-28mm-minimal.stl -D depth=28;showCover=true;showBase=false;showPcb=false;showSwitch=false;minimalCover=true JediLightSwitch.scad
"C:\Program Files\OpenSCAD\openscad.com" -o Base-28mm.stl -D depth=28;showCover=false;showBase=true;showPcb=false;showSwitch=false JediLightSwitch.scad

@echo Building 25mm
REM 25mm depth (old style switch) - minimal
"C:\Program Files\OpenSCAD\openscad.com" -o Cover-25mm-V1.1.stl -D depth=25;showCover=true;showBase=false;showPcb=false;showSwitch=false;minimalCover=false;pcbVersion=1 JediLightSwitch.scad
"C:\Program Files\OpenSCAD\openscad.com" -o Cover-25mm-V1.2.stl -D depth=25;showCover=true;showBase=false;showPcb=false;showSwitch=false;minimalCover=false;pcbVersion=2 JediLightSwitch.scad
"C:\Program Files\OpenSCAD\openscad.com" -o Cover-25mm-minimal.stl -D depth=25;showCover=true;showBase=false;showPcb=false;showSwitch=false;minimalCover=true JediLightSwitch.scad
"C:\Program Files\OpenSCAD\openscad.com" -o Base-25mm.stl -D depth=25;showCover=false;showBase=true;showPcb=false;showSwitch=false JediLightSwitch.scad

@echo Building 19mm
REM 19mm depth (new style switch) - easy fit
"C:\Program Files\OpenSCAD\openscad.com" -o Cover-19mm-V1.1.stl -D depth=19;showCover=true;showBase=false;showPcb=false;showSwitch=false;minimalCover=false;pcbVersion=1 JediLightSwitch.scad
"C:\Program Files\OpenSCAD\openscad.com" -o Cover-19mm-V1.2.stl -D depth=19;showCover=true;showBase=false;showPcb=false;showSwitch=false;minimalCover=false;pcbVersion=2 JediLightSwitch.scad
"C:\Program Files\OpenSCAD\openscad.com" -o Cover-19mm-minimal.stl -D depth=19;showCover=true;showBase=false;showPcb=false;showSwitch=false;minimalCover=true JediLightSwitch.scad
"C:\Program Files\OpenSCAD\openscad.com" -o Base-19mm.stl -D depth=19;showCover=false;showBase=true;showPcb=false;showSwitch=false JediLightSwitch.scad

@echo Building 16mm
REM 16mm depth (new style switch) - minimal
"C:\Program Files\OpenSCAD\openscad.com" -o Cover-16mm-V1.1.stl -D depth=16;showCover=true;showBase=false;showPcb=false;showSwitch=false;minimalCover=false;pcbVersion=1 JediLightSwitch.scad
"C:\Program Files\OpenSCAD\openscad.com" -o Cover-16mm-V1.2.stl -D depth=16;showCover=true;showBase=false;showPcb=false;showSwitch=false;minimalCover=false;pcbVersion=2 JediLightSwitch.scad
"C:\Program Files\OpenSCAD\openscad.com" -o Cover-16mm-minimal.stl -D depth=16;showCover=true;showBase=false;showPcb=false;showSwitch=false;minimalCover=true JediLightSwitch.scad
"C:\Program Files\OpenSCAD\openscad.com" -o Base-16mm.stl -D depth=16;showCover=false;showBase=true;showPcb=false;showSwitch=false JediLightSwitch.scad

@echo Building skinny test cover
REM 5mm depth (new style switch) - test print
"C:\Program Files\OpenSCAD\openscad.com" -o Cover-5mm-V1.1.stl -D depth=5;showCover=true;showBase=false;showPcb=false;showSwitch=false;minimalCover=false;pcbVersion=1 JediLightSwitch.scad
"C:\Program Files\OpenSCAD\openscad.com" -o Cover-5mm-V1.2.stl -D depth=5;showCover=true;showBase=false;showPcb=false;showSwitch=false;minimalCover=false;pcbVersion=2 JediLightSwitch.scad
"C:\Program Files\OpenSCAD\openscad.com" -o Cover-5mm-minimal.stl -D depth=5;showCover=true;showBase=false;showPcb=false;showSwitch=false;minimalCover=true JediLightSwitch.scad
"C:\Program Files\OpenSCAD\openscad.com" -o Base-5mm.stl -D depth=5;showCover=false;showBase=true;showPcb=false;showSwitch=false JediLightSwitch.scad

// -----------------------------------------------------------------
// Jedi Light Switch Enclosure.
// Designed to be an enclosure for the Jedi Light Switch PCB
// and fit over a UK (Possibly EU) Lightswitch.
// -----------------------------------------------------------------

// Define the switch type to be covered.
// This only effects the switch modeled, not the depth of the case.
// New style Single switch = 0
// New style Double switch = 1 - not implemented.
// Old style Single switch = 2
// Old style Double switch = 3
switchType  = 0; // Update depth also

// if new style switch
// depth = 16; // 19mm Gives plenty of room in new switches.
// if old style
depth = 25; // 28mm gives plenty
// Supper skinny - helpful for test print to check sizes.
// depth = 5;

minimalCover = false;

// How deep to make the USB connector cutout.
// For ease of use this should be the same as, or deeper than 
// the depth of the cover so the cover can be fitted/removed 
// without disconnecting the USB connector.
usbConnectorCutoutDepth = depth;

// The gap to leave between the cover and the base when pushed together.
// Adjust this to give a tighter (smaller number) or looser (larger number) fit between
// the base and the cover. This will depend on your printer.
coverToBasegap = 0.2;

// Change these to show the various main components.
// They are modified through command line calls when generating STL files.
showCover = true;
showBase = false;
showPcb = false;
showSwitch = false;


// ------------------------------------------------------


            
// Size of the outer bezel that surounds the PCB
bezelSize = 2;

// Thickness of the cover wall.
// PCB overlap onto support = 2
// then add the bezel size to allow for an overall wall thickness.
wallThickness = 2 + bezelSize; // top wall thickness.
sideWallThickness = 2 + bezelSize;

// -----------------------------------------------------------------
// Jedi lightswitch PCB details
// -----------------------------------------------------------------
// PCB size we are trying to fit.
// PCB is 100, allow a small bodge
pcbHeight = 100.25;
pcbWidth = 100.25;
// How thick (deep) the front panel is (1.6mm FR4 PCB)
pcbThickness = 1.6;
// -----------------------------------------------------------------

// -----------------------------------------------------------------
// Compute the actual overall size required 
// for the outer enclosure.
// -----------------------------------------------------------------
height = pcbHeight + (2*bezelSize);
width = pcbWidth + (2*bezelSize);
// -----------------------------------------------------------------

// -----------------------------------------------------------------
// Wall Switch size (physical size of the light switch being covered)
// -----------------------------------------------------------------
switchHeight = pcbHeight-15; //88;
// switchWidth = 88;  // Original value.
switchWidth = 85; // Newer switches.
// How deep the switch is at the deepest part (e.g. to the top of the switches switch).
switchDepth = 22;
// How much the switch body should overlap onto the base
switchOverlap = 3;

middleDepth = 10;

// How deep the inside of the box is.
baseInnerThickness = 1;

// -----------------------------------------
// -----------------------------------------
module GenericBase(xDistance, yDistance, zHeight) {
	// roundedBox([xDistance, yDistance, zHeight], 2, true);

	// Create a rectangluar base to work from that
	// is xDistance by yDistance and zHeight height.

	// This is effectivly a cube with rounded corners

	// extend the base out by 3.5 from holes by using minkowski
	// which gives rounded corners to the board in the process
	// matching the Gadgeteer design
	
	$fn=50;
	radius = 5; //bezelSize;


	translate([radius,radius,0]) {
		minkowski()
		{
			// 3D Minkowski sum all dimensions will be the sum of the two object's dimensions
			cube([xDistance-(radius*2), yDistance-(radius*2), zHeight /2]);
			cylinder(r=radius,h=zHeight/2);
		}
	}
}



// -----------------------------------------
// Base for the switch, goes around existing lightswitch.
// -----------------------------------------
module LightSwitchBase() {
	
	difference() {
		union() 
		{
			// Outer base wall
			OuterWall();
		}		
		union() 
		{
			SwitchCutout();
			PcbCutout();
            CutOff();
		}
	}
}

// -----------------------------------------
// Main base outer wall
// -----------------------------------------
module OuterWall() {

    innerCutoutOffset = wallThickness; // bezelSize + x

	difference() {
		union() {
			GenericBase(width, height, depth);
		}
		union() {
			// Cut out the bulk of the inside of the box.
			translate([sideWallThickness, wallThickness, baseInnerThickness]) {
                
                // Rounded interial
				//GenericBase(width - (wallThickness* 2), 
				//					height - (innerCutoutOffset *2), 
				//					(depth - baseInnerThickness) + 1);
                
                // Square interia
                cube([width - (sideWallThickness * 2), 
                    height - (wallThickness *2), 
					(depth - baseInnerThickness) + 1]);
			}
		}
	}
}

// -----------------------------------------
// Cut out an area in the base for the switch allowing
// for an overlap to go under the switch which holds the case to the wall.
// -----------------------------------------
module SwitchCutout() {

	// Cutout smaller than the actual switch to allow for the overlap all around
	cutoutWidth = switchWidth - (switchOverlap*2);
	cutoutHeight = switchHeight - (switchOverlap*2);	

	// Padding either side of the cutout.
	paddingWidth = (width - cutoutWidth) / 2;
	paddingHeight = -10; // Fixed padding from top.

	// Switch cutout.
	// Cut out a area less wide than the switch so it sits on it 
	// keeping the box against the wall
	// -1 z to ensure it goes all the way through
	translate([paddingWidth, paddingHeight, -1]) {
		cube([cutoutWidth, height, 4]);
	}

	// Switch body
	// Create a block to show how the switch body sits
	// in the base.
	switchOuterPaddingWidth = (width - switchWidth) /2;
	switchOuterPaddingHeight = paddingHeight - switchOverlap;

	translate([switchOuterPaddingWidth , switchOuterPaddingHeight,1]) {
		color( [1, 0, 0, 0.90] ) {
            cube([switchWidth, switchHeight, switchDepth]);
		}
	}
}

// -----------------------------------------
// Cut out an area in the base for the PCB.
// -----------------------------------------
module PcbCutout() {
	// Move to a slight offset to allow for an outer bezel.
	// and position so the top of the pcb is at the top of the base box.
	translate([bezelSize, bezelSize, depth - (pcbThickness - 0.1)]) {
		Pcb();        
	}
}

// -----------------------------------------
// Generate a model of the PCB.
// -----------------------------------------
module Pcb() {
        photonWidth = 24;
        photonHeight = 37;
        photonDepth = 3.5;
        
        // PCB.
        color( "purple" ) {
            GenericBase(pcbWidth, pcbHeight, pcbThickness);
        }
        
        // Photon
        translate([(pcbWidth/2) - (photonWidth /2),0 , -(photonDepth)]) {
            cube([photonWidth, photonHeight, photonDepth]);
        }
        
        // LEDs
        ledOffsetFromSide = 3;
        ledOffsetFromTopBottom = 1.5;
        ledWidth = 3; // Include the resistor and LED and a small fudge for the oversized PCB.
        ledHeight = 7;
        ledDepth = 1;
        
        color( "blue" )  {
            // Top left LED
            // 0,0,0 bottom left.
            translate([
                        0 + ledOffsetFromSide,
                        pcbHeight - ledOffsetFromTopBottom - ledHeight,
                        -ledDepth]) {
                cube ([ledWidth, ledHeight, ledDepth]);
            }
           
            
            // top right LED
            translate([
                        pcbWidth - ledOffsetFromSide - ledWidth,
                        pcbHeight - ledOffsetFromTopBottom - ledHeight,-ledDepth]) {
                cube ([ledWidth, ledHeight, ledDepth]);
            }
            
            // Bottom Left LED.
            translate([
                    0 + ledOffsetFromSide,
                    ledOffsetFromTopBottom,
                    -ledDepth]) {
                cube ([ledWidth, ledHeight, ledDepth]);
            }
            
            // Bottom Right LED.
            translate([
                    pcbWidth - ledOffsetFromSide - ledWidth,
                    ledOffsetFromTopBottom,
                    -ledDepth]) {
                cube ([ledWidth, ledHeight, ledDepth]);
            }
            
            // Show LED holes with LEDs shining through
            // LED hole
            translate([
                        0 + 5,
                        pcbHeight - 5,
                        -0.1]) {
                cylinder(d=2.5, h=3, $fn=10);
            }
            
            translate([
                        pcbWidth - 5,
                        pcbHeight - 5,
                        -0.1]) {
                cylinder(d=2.5, h=3, $fn=10);
            }
            
            translate([
                        0 + 5,
                        5,
                        -0.1]) {
                cylinder(d=2.5, h=3, $fn=10);
            }
            
            translate([pcbWidth - 5,
                        5,
                        -0.1]) {
                cylinder(d=2.5, h=3, $fn=10);
            }
        }
        
        // components for sensors on the base of the board.
        // V1.1 this is not such as issue as they are 4 mm from edge.
        translate([(pcbWidth/2) -15 ,pcbHeight - 25, -pcbThickness]) {
            cube([44, 20, ledDepth + pcbThickness]);
        }
        
        // Humidity and gesture components on the board top.
        // 2.5mm offset from board top.
        // 10mm heihg
        // 6mm wide
        // actually 2mm hight but project 5mm to make it stand out.
        translate([(pcbWidth/2) -3 ,pcbHeight - 10 - 2.5, pcbThickness]) {
            color( "black" ) cube([6, 10, 2]);
        }
        
        // Small raised area to indicade the touch pad.
        // touch pad is 19mm from top.
        // 40mm height
        // 85mm wide
        // 8mm from left/right edge.
        translate([8,pcbHeight -40- 19, 0]) {
            color( "SteelBlue" )  {
                cube([85, 40, pcbThickness + 1]);
            }
        }
        
        // V1.1 of PCB no longer has screw holes.
        includeScrewHoles = false;
        // Screw holes
        if (includeScrewHoles) {
            // All the way through to prevent trapped resin on Form printer.
            translate([(pcbWidth/2) - 48 ,(pcbHeight/2), -(depth+1)]) {
                cylinder(r=1.25, h=depth + 3, $fn=40);
            }
        
            translate([(pcbWidth/2) + 48,(pcbHeight/2), -(depth+1)]) {
                cylinder(r=1.25, h=depth + 3, $fn=40);
            }
        }
}

// -----------------------------------------
// Cutoff the end of the switch to allow it to be pushed over the lightswitch 
// and slid down. The outer cover will hide this missing piece.
// -----------------------------------------
module CutOff() {     
    // +2mm on switch width to allow for a small tolerance.
    cutoutOffset = (width - (switchWidth+2)) / 2;
    cutoutWidth = width - (wallThickness*2) - (switchOverlap*2) + 2*2;
    
    translate([cutoutOffset,0,0]) {
        cube([switchWidth+2,wallThickness  ,depth+2]);
    }
}

// -----------------------------------------
// This is the outer cover that goes over the base and
// holds the PCB in place.
// -----------------------------------------
module Cover() {
    coverWallThickness = 2;
    
    coverWidth = width + coverWallThickness*2;
    coverHeight = height + coverWallThickness*2;
        
    // Cover depth to match the overall depth of the base
    // plus the additional cover top thickness
    // then allow 1mm gap for wall edge variances so it's a snug-ish fit
    // ensuring cover goes fully onto base.
    coverWallOffset = 1;
    coverDepth = depth + coverWallThickness - coverWallOffset;
    coverTopThickness = 1;
       
    usbHeight = 10; // Height here is in the z axid.
    usbWidth = 13;
    
    // How much over the top (sensors) the cover should cover.
    coverTopOverlap = 21;
        
    includeBottomVentHoles = true;
    ventWidth = width - 40;
    ventHeight = 8;
    
    difference() {
		union() {
			GenericBase(coverWidth, coverHeight, coverDepth);
		}
		union() {
            // Hollow out to fit around the base.
            translate([coverWallThickness - coverToBasegap, 
                            coverWallThickness - coverToBasegap, 
                            -coverTopThickness]) {
                    GenericBase(width + (coverToBasegap*2), height + (coverToBasegap*2), coverDepth);
            }
            
            // Cutout for the USB connector...
            translate([(coverWidth/2)-(usbWidth/2),
                        coverHeight - coverWallThickness - 1,
                        depth-(coverTopThickness + usbConnectorCutoutDepth)]) {
                cube([usbWidth,coverWallThickness+2,usbConnectorCutoutDepth]);
                // TODO: Put cylinders on ends to make it rounded.
            }
            
            // Add some vent holes in the bottom to allow air
            // circulation and for light to escape when using "Glow"
            if (includeBottomVentHoles) {
                translate([(coverWidth/2)-(ventWidth/2),
                        coverHeight - coverWallThickness - 1,
                        0]) {
                    #cube([ventWidth,coverWallThickness+2,ventHeight]);
                }
            }
                
            if (minimalCover) {
                // Allow for cover thickness, the base bexel size.
                // base has 2mm PCB overlap
                overlap = coverWallThickness + bezelSize + 2;
                doubleOverlap = overlap * 2;
                
                // Cut out the main PCB area
                translate([overlap,overlap,+2]) {
                    GenericBase(coverWidth-doubleOverlap, coverHeight- doubleOverlap, coverDepth);
                }
            } else {
                coverCutouts(coverWidth, 
                                coverHeight, 
                                coverWallThickness, 
                                coverTopThickness,
                                coverDepth, 
                                coverTopOverlap, 
                                coverWallOffset);
            }
        }
	}
}

// -----------------------------------------
// Defines the parts of the cover to be cutout.
// -----------------------------------------
module coverCutouts(coverWidth, 
                    coverHeight, 
                    coverWallThickness, 
                    coverTopThickness,
                    coverDepth, 
                    coverTopOverlap, 
                    coverWallOffset) {

    // How far down from the top of the cover the opening for the sensor set should be.
    sensorSetTopOffset = 3.5;
    sensorSetHeight = 12;
    sensorSetWidth = 8;
    
    // How much over the bottom the cover should cover.
    // this hides the non-touchable zone (where the Photon would mess
    // with tuch sensor.
    coverBottomOverlap = 43;
    
                // Cut out the main touch switch area.
                // Gives an overlap of 10mm on the PCB left and right
                // and a larger overlap at the top to cover the sensor area.
                // and matching overlap at the bottom to give it some sym.
                translate([10,coverTopOverlap,+2]) {
                    GenericBase(coverWidth-20, coverHeight- (coverTopOverlap + coverBottomOverlap ), coverDepth);
                }
                
                // Cout out the space for the sensors.
                // move down coverWallThickness + 2mm from top so some PCB is covered.           
                translate([(coverWidth/2),
                            coverWallThickness + sensorSetTopOffset + (sensorSetWidth/2)-1,
                            depth-coverTopThickness + coverWallOffset]) {
                    #cylinder(d=sensorSetWidth, h=coverTopThickness, $fn=20);
                }
                
                translate([(coverWidth/2),
                            coverWallThickness + sensorSetTopOffset + sensorSetHeight - (sensorSetWidth/2) + 1,
                            depth-coverTopThickness + coverWallOffset]) {
                    #cylinder(d=sensorSetWidth, h=coverTopThickness, $fn=20);
                }
                
                // Join the two cylinders to make the cutout oblong
                translate([(coverWidth/2)-(sensorSetWidth/2),
                            coverWallThickness + sensorSetTopOffset + 3,
                            depth-coverTopThickness + coverWallOffset]) {
                    #cube([sensorSetWidth,sensorSetHeight-6,coverTopThickness]);
                }
                                
                // Don't include LED holes as material is most likely thin enough
                // for the LEDs to shine through without needing extra holes.
}

// -----------------------------------------
// Show the model of the switch.
// NB: Do not include when generating stl's
// -----------------------------------------
module Switch(xOffset, yOffset, zOffset) {
    translate([xOffset, yOffset, zOffset]) {
        color( [0, 0, 1, 1] ) {
                       
            if(switchType == 0) {            
                // New style switches
                switchBodyDepth = 8.5;
                switchSwitchDepth = 4; // on top of the body.
                
                // Square switch base
                // Switch depth = 8.5mm.
                cube([switchWidth, switchHeight, switchBodyDepth]);
                
                // Put the actual switch switch on the base.
                translate([(switchWidth /2) - 8, (switchHeight/2) - 13, switchBodyDepth]) {
                    cube([16, 26, switchSwitchDepth]);
                }
            } else if(switchType == 2) {
                // Old style lever switch
                
                // Square switch base
                cube([switchWidth, switchHeight, 16]);
                
                // Put the actual switch switch on the base
                // Show switch on the on position only as it would be expected tobe.
                // Make switch y=22mm for full range.
                translate([(switchWidth /2) - 4, (switchHeight/2)-11, 16]) {
                    cube([8, 11, 8]);
                }
            } else if(switchType == 3) {
                // Old style lever switch
                
                // Square switch base
                cube([switchWidth, switchHeight, 16]);
                
                // Put the actual switch switch on the base
                // Show switch for On & Off as either switch
                // may be in that state as this is typically
                // a landing/hall switch and has a second switch
                translate([(switchWidth /2) - 13, (switchHeight/2)-11, 16]) {
                    cube([26, 22, 8]);
                }
            }
		}
    }
}





// -----------------------------------------
// MAIN
// -----------------------------------------

if (showBase) {
    LightSwitchBase();
}

if (showCover) {
    // Case top rotated 180° (well openings swapped around.
    // so rotate and adjust position os they sit ontop.
    rotate(180) {
        
        // Adjust x offset to move cover up/down onto base.
        translate([-width -2.2,-height -2.2,0]) {
            color( [0, 1, 0, 1] ) {
               Cover();
            }
        }
    }
}

// Optional models to help visualise the enclosure.

if (showSwitch) {
    // Show the actual light switch we are covering
    // comment out when generating STLs.
    switchX = (width - switchWidth) /2;
    Switch(switchX,14,1);
}

if (showPcb) {
    // Show the PCB (in purple)
    // comment out when generating STLs.
    translate([bezelSize, bezelSize, depth - (pcbThickness - 0.1)]) {
        Pcb();        
    }
}
Jedi LightSwitch
================

The Jedi Light Switch is a home automation light switch which responds to the Jedi wave or touch(1). It aims to bring the humble wall switch into the modern WiFi lighting era.

1) Automation of modern WiFi lamps (LIFX/Hue) doesn't work if the lamp is switch off at the wall switch.
2) Existing switch may not work as expected for visitors with WiFi bulbs. Often the switch is on but the light off.
3) Switching the lights on/off via an app is annoying.
4) The wall switch may be difficult or impossible to replace with a automation friendly version. (Excludes the Jedi Lightswitch :-) )
5) The existing switch is single use, it switches on or off the main light. In a modern house their are many things this switch can be doing.

The Jedi Light Switch is PCB based with a custom 3D printed enclosure. It is powered by a Particle Photon which uses WiFi and optionally cloud services (i.e. IFTTT.com / Tinamous.com) to control and monitor the room environment.

The Photon may be user programmed so the switch may be heavily customised as desired, even OTA after installation. Likewise lighting control via IFTTT may be modified at will.

As well as acting as a switch it also acts as a room sensor with built-in temperature, humidity and light level measurements which may be used for further automation tasks.

Proximity detection allowing the sensing if somebody is close to the switch. (e.g. Switch the lights on in the room if they are not and somebody is close to the switch as they are probably about to try and switch them on anyway).

Two LEDs controlled by the Photon give the immediate feedback (cloud sevices may take a few seconds to perform the apporpiate operations), these LEDs may also be internet controlled to draw attention to something (i.e. the doorbell ringing).

Why:
----
If you have LIFX or Hue lights they stop working once somebody switches them off at the wall switch which can ruin your automation plans. The Hue has a small switch however the LIFX lights rely on 3rd party products for switching, both suffer from our built-in tendandy to switch the light off at the switch as we leave the room. Neither lights work so well when that happens! Neither Hue or LIFX switch options are immediatly obious that they should be used in preference to the wall switch. Nobody wants to have to train their guests how to switch the lights on or off.

Imaging the scene. You walk into your room, the lights are off but the light switch is on.
* Option 1: Switch the light switch off then on again. This works a charm (but only for the main light).
* Option 2: 
** Find your phone (hopefully it's not somewhere in the dark room you are walking into)
** Unlock your phone
** Launch the LIFX application
** Wait whilst the LIFX applicaiton loads
** Try not to trip or walk into anything as you've continued to walk whilst working your phone in the darkened room
** Scroll to the bulbs or scene you wish to set
** Select the bulbs or scene
** Hopefully the lights respond first time, if not wait a bit and try again
** Put phone away
** Think how great it is you can control your lights from your phone...

Leaving a room:
---------------
Nobody likes to leave the lights on in rooms that are unused or when you go out.....

Imaging you leave the room.
* Option 1: Flick the light switch to switch the light off (goodbye automation).
* Option 2: Find your phone, unlock your phone (you see where this is going).

Neither of these senarios are great. You could replace or remove the light switch...

The Light Switch Problem:
-------------------------
* In the UK we have a different style and voltage requirement for light switchs compared to USA, hence home automation products are slow (if ever) to reach the UK market.
** This is not helped by the need for a CE mark which is slow and expensive.
** Often the switches that are available are extreamly cheap (I've had 5x £30 switches fail when the incandecent bulb blew).
* Most HA light switches require a minimum of 25mm backing box. A lot of UK homes use a 15mm box which either means you need to pad the switch out on the wall (ugly) or replace the box (painful).
* A lot (almost all) HA light switches require "3 wire" configuration. Live, Neutral and switch return. Most UK homes have only 2 wire switches (Live and return).
* Not everybody wants to play with the electrics to get a fancy light switch fitted or pay for an expensive electrician.
** If it's a 2 wire replacement this isn't too difficult, changing the 2 wires to 3 and/or replacing the backing box are a big job.
* If you rent chances are you are not allowed to replace the light switch anyway. Who wants to get thrown out of their rented accomodation for a bit of home automation goodness!
* Switching the lights off at the light switch ruins automation.
* The light switch generally only controlls the main light, occasionally wall lights.
* Switching the wall switch on or off doesn't tell your home automation system that the action has happened so no further actions can happen based on that.
* Theirs no intelligence in the on/off of the humble wall switch. If you walk into a room when you're still half asleep having the light come on at 100% brightness is painful.

The Solution:
-------------
The Jedi Light Switch is designed to remove those problems through a two part solution. The main part is the touch and gesture sensitive WiFi based switch PCB, the second is the enclosure.

The switch and enclosure are designed to be simple to install, no re-wiring required (except for external USB power), a direct replacement for the humble wall switch whilst leaving the switch in place, just hidden behind some sexy technology.

The current enclosure is designed to fit over a UK light switch (sorry rest of the world, It may work in Europe, it certainly won't fit over USA switches). 

The enclosure is 3D printed in two parts:

1) The base. This has an overlap area that fits between the wall and the existing switch to secure the base in place and only requires the switch to be loosened and not removed.
** To fit loosen the screws on the light switch slightly so that the switch comes forward about 2mm.
** Slide the base down the wall so the back fits between the wall and the switch until the top of the switch is as close to the top of the base as it can go.
** Tighten the switch screws, ensuring the enclosure is level. This clamps the base part to the wall. No other fixing is required.
2) The cover:
** This is optional. You may if you wish attach the switch PCB via glue or screws to the base and be done.
** The cover gives a nicer finish to the switch whilst also providing mechanical fixing of the PCB within the base. It may be printed in any colour you can get the 3D filament or resin for, although clear works well.
** Optional: You may also print on a clear "acetate film" to fit between the cover and the PCB with a nice pattern or design over the touch sensor.
** The cover simply slides over the base with the PCB in the middle and stays in place by friction making it easy to remove should you need access to the actual wall switch.

Other Advantages:
* Use of gesture to switch the lights on would work well in dirty/wet areas (e.g. the kitchen) where you may not wish to touch something (e.g. you've half way through cooking chicken).
* The switch supports up, down, near and far waves as well, these may be used to dim the lights, control additional (hall/landing) lighting or something else.
* You don't actually need an existing light switch. The case could be directly attached to the wall or something else to provide easy additional lighting points.
* Other than a USB power pack and WiFi no other external parts are required (no Hub, no dedicated lighting wiring, no ......, not even screws or double sided sticky tape)
* You don't have to be British (but it helps). The switch enclosure and PCB are sized to cover a UK light switch, however as they can be used in stand-alone mode they don't have to be used with a UK switch.
** The SCAD design for the enclosure is provided which may be modified to fit other requirements.
* Intellegent:
** The lights could be dimmed to brightness greadually, especially if the switch knew the room was very dark previously.
** or set to automatically switch off or dim the lifhts after 2 hours.
** or the air-con switched on if the room is hot and humid and the lights have just been switched on (or off).
** the curtains could be closed automatically when the lights are switched on.
** the whole house lights could be switched off if the hall light was switched off (and vica versa, house lights switched on when it's dark when hall/door switch is switched on).


Limitation:
-----------

* Currently requires external USB 5V DC power as the Photon is relativly fairly power hungry device (i.e. using WiFi). Even if a battery was fitted it would require a fair size and to be topped up regularly particularly if the switch is required to respond to network actions (e.g. light LEDs, update firmware etc.).

* Lack of a traditional switch mechanism which may confuse guests. Hopefully the touch ability would be enough to make up for this.

* Cloud services can be slow. Using Photon -> Particle.io -> IFTTT -> LIFX.com -> LIFX bulb can be seconds in the trip which is not idea for a light switch. In this time a new user may have told the switch to switch on/off/on/off/on etc a number of times.


Future upgrades:
----------------

Planned for V1.1:
* 2 LEDs in the bottom corners to match the top so each corner has an LED.
* Back lighting. Adding more LEDs to project backwards to light up the enclosure (works best for clear enclosures). Ideally NeoPixel type so they can be controlled from 1 wire and have a range of colour / intensity controlled.

Future posabilities:
* Split/Dual touch sensor pads so that it works for hall/landing lights (up/down) or duel lighting modes in a room (main/wall lights).
* RS485 interface, supply 5V to Photon from a RS485 with 12V house wired RS485 network, interface with other RS485 HA devices and provide a more reliable connection and power.
** This is easier then putting a PoE Ethernet connection on the switch.
* Battery + charger
* Improved interrupt's that can be used to wake the Photon (currently 2 interupts, one high and one low which can't both be used to wake the Photon) to improve battery life.
* Buzzer
* IR Sender to allow the switch to send out instructions from automation requests (e.g. telly on) or relay the signal from room to room.
* IR Receiver to allow remove control of the switch and/or transmission from room to room.
* Microphone (to decect noise levels rather than listen).
* PIR sensor
* Improved temp/humidity sensor for one of the new Bosch line with BP and gas sensing as well.

License:
--------
Licensed under Creative Commons ShareAlike 4.0 https://creativecommons.org/licenses/by-sa/4.0/

Restrictions: May not be used to control lighting on a death star.

Notes:
------
1) Also responds to non-Jedi waves and touch. Not to be used where control by non-Jedi personel is restricted.
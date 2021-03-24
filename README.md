# micro-vhf-uhf-repeater

this is small cross band repeater for drones installtion.
it runs on 5V battery and has support for one lipo cell 3.4V-4.2V

the repeater can work both ways (v2u & u2v) and has 0.8W - 1W of output power

DTMF:
there is place on the PCB for DTMF module (2 common versions) and it is connected to the arduino. the arduino sketch does not support it yet.

POWER:
there ae 2 ways to power the repeater :

1) via the TYPE C conector - connect it to power bank (1.5A on TX)
2) via 1 LIPO CELL (lower output power)

once you use battery, dc-dc converter get's in to duply 5V to the arduino and to DTMF module, but I have noticed that some arduinos can work 3.4V too
if your arduino can handle 3.4V - don't solder the dc-dc module and short it with wire.

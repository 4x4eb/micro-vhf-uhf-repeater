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

components:
1) cap component are 1206 SMT = 10uF
2) resistors component are 1206 SMT = 100-150 ohm.
3) pot are 10K
4) schottky diode - RB751V40T1G or like(any diode with low Vf)
5) dc-dc module -  S7V7F5 

about tone:

 1=67, 2=71.9, 3=74.4, 4=77, 5=79.7, 6=82.5, 7=85.4, 8=88.5, 9=91.5,10=94.8, 11=97.4");
 12=100, 13=103.5, 14=107.2, 15=110.9, 16=114.8, 17=118.8, 18=123, 19=127.3, 20=131.8");
 21=136.5, 22=141.3, 23=146.2, 24=151.4, 25=156.7, 26=162.2, 27=167.9, 28=173.8, 29=179.9");
 30=186.2, 31=192.8, 32=203.5, 33=210.7, 34=218.1, 35=225.7, 36=233.6 , 37=241.8, 38=250.3");



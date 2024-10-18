# simple-nfc-lock
a very very simple nfc lock based on arduino uno 

btw i cpoied the original code from a website but i lost the bookmark for it, so if u write the original part i'm sorry for this and plz contact me！！！
# How it works
coil reads a card >> board verfiy the card id >> turn the servo if passed >> supersonic sensor monitoring distance between lock and wall >> turn the servo back if supersonic data indicate that door has been open
# the components that i used
Arduino uno: logic board here

MFRC522: for nfc

MG90s: lever, for lock that require force less than 14g

HC-SR04: supersonice module, measure distance between lock and wall, which indicate if door is open or not

a out of no where battery: just grab a used powerbank, and it should be fine

3d printed outter shell: if u go with my design plz DOBBLE CHECK THE SIZE, because the one that i use isn't common size 

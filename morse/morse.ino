#include <LedControl.h>
#include "char_table.h"

#define BTN_PIN 3
#define BUZ_PIN 4
#define DIN 0
#define CS  1
#define CLK 2
#define DOT_THRES 200
#define TIMEOUT 400
#define BEEP_FREQ 845
#define LOOP_DELAY 25

unsigned long start_time_press, start_time_unpress;
byte last_press;
bool is_dot = true, timeout=false;
String letter;

LedControl lc=LedControl(DIN, CLK, CS,0);

void setup() {
	pinMode(BTN_PIN, INPUT);
	lc.shutdown(0,false);
	lc.setIntensity(0,0);
	lc.clearDisplay(0);
}
  
void loop() {
	byte pressed = digitalRead(BTN_PIN);

	if (pressed&&!last_press) {
		tone(BUZ_PIN, BEEP_FREQ);
		start_time_press = millis();
		timeout = false;
	}

	if (!pressed&&last_press) {
		noTone(BUZ_PIN);
		start_time_unpress = millis();
		if (is_dot) letter += ".";
		else        letter += "-";
		is_dot = true;
	}

	if (pressed) {
		if (millis()-start_time_press>=DOT_THRES) is_dot=false;
	} else {
		if (millis()-start_time_unpress>=TIMEOUT) {
			if (!timeout) {
				bool found = false;
				for (int i=0; i<sizeof(table)/sizeof(table[0]); i++) {
					if (!strcmp(letter.c_str(), table[i].morse)) {
						found = true;
						for (int j=0; j<8; j++) {
							byte index = table[i].ascii[0];
							if (index>=65) {
								lc.setRow(0, j, char_table[index-65][j]);
							} else {
								lc.setRow(0, j, char_table[index-22][j]);
							}
						}
         			}
					if (!found)
						lc.clearDisplay(0);
				}
				letter = "";
			}
			timeout=true;
			is_dot = true;
		}
	}

	last_press = pressed;

	delay(LOOP_DELAY);
}

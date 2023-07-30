#ifndef buzzer_included
#define buzzer_included

void buzzer_init();
void buzzer_set_period(short cycles);

void play4D();
void play4F();
void play4Fsharp();
void play4G();
void play4A();
void play4Csharp();
void play5C();
void playState(short playTheme, short interrupts);

#endif // included

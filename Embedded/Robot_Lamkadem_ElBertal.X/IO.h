#ifndef IO_H
#define IO_H

//Affectation des pins des LEDS
#define LED_BLANCHE_1 _LATJ6
#define LED_BLEUE_1 _LATJ5
#define LED_ORANGE_1 _LATJ4 
#define LED_ROUGE_1 _LATJ11
#define LED_VERTE_1 _LATH10

#define LED_BLANCHE_2 _LATA0
#define LED_BLEUE_2 _LATA9
#define LED_ORANGE_2 _LATK15
#define LED_ROUGE_2 _LATA10
#define LED_VERTE_2 _LATH3


//D�finitions des pins pour les 6 hacheurs moteurs

#define MOTEUR_GAUCHE_INL MOTEUR1_IN1
#define MOTEUR_GAUCHE_INH MOTEUR1_IN2
#define MOTEUR_GAUCHE_ENH IOCON1bits.PENL
#define MOTEUR_GAUCHE_ENL IOCON1bits.PENH
#define MOTEUR_GAUCHE_DUTY_CYCLE PDC1

#define MOTEUR_DROIT_INL MOTEUR6_IN1
#define MOTEUR_DROIT_INH MOTEUR6_IN2
#define MOTEUR_DROIT_ENH IOCON6bits.PENL
#define MOTEUR_DROIT_ENL IOCON6bits.PENH
#define MOTEUR_DROIT_DUTY_CYCLE PDC6

//D�finitions des pins pour les hacheurs moteurs
#define MOTEUR1_IN1 _LATE0 // � 1 pour sens inverse
#define MOTEUR1_IN2 _LATE1 // � 1 pour sens normal

#define MOTEUR2_IN1 _LATE2 // � 1 en sens normal
#define MOTEUR2_IN2 _LATE3

//Configuration sp�cifique du moteur gauche
#define MOTEUR_GAUCHE_H_IO_OUTPUT MOTEUR1_IN1
#define MOTEUR_GAUCHE_L_IO_OUTPUT MOTEUR1_IN2
#define MOTEUR_GAUCHE_L_PWM_ENABLE IOCON1bits.PENL
#define MOTEUR_GAUCHE_H_PWM_ENABLE IOCON1bits.PENH
#define MOTEUR_GAUCHE_DUTY_CYCLE PDC1

//Configuration sp�cifique du moteur droit
#define MOTEUR_DROIT_H_IO_OUTPUT MOTEUR6_IN1
#define MOTEUR_DROIT_L_IO_OUTPUT MOTEUR6_IN2
#define MOTEUR_DROIT_L_PWM_ENABLE IOCON6bits.PENL
#define MOTEUR_DROIT_H_PWM_ENABLE IOCON6bits.PENH
#define MOTEUR_DROIT_DUTY_CYCLE PDC6

// Prototypes fonctions
void InitIO();
void LockIO();
void UnlockIO();

#endif /* IO_H */

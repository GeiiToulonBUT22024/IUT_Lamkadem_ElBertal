/* 
 * File:   main.c
 * Author: TP_EO_6
 *
 * Created on 13 septembre 2023, 14:12
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "ADC.h"
#include "robot.h"
#include "main.h"

unsigned int * result;
int ADCValue0 = 0;
int ADCValue1 = 0;
int ADCValue2 = 0;

int main(void)
{

    /***************************************************************************************************/
    //Initialisation de l?oscillateur

    /****************************************************************************************************/
    InitOscillator();
    InitIO();


    InitADC1();
    InitTimer23(); // Initialisation des timers
    InitTimer1();
    InitTimer4();
    /****************************************************************************************************/
    // Configuration des �entres sorties
    /****************************************************************************************************/

    LED_BLANCHE = 0;
    LED_BLEUE = 0;
    LED_ORANGE = 0;

    InitPWM();
    //    PWMSetSpeed(20, MOTEUR_GAUCHE);
    //    PWMSetSpeed(20, MOTEUR_DROIT);
    /****************************************************************************************************/
    // Boucle Principale
    /****************************************************************************************************/


    while (1) {
        if (ADCIsConversionFinished() == 1) {
            ADCClearConversionFinishedFlag();
            unsigned int * result = ADCGetResult();
            float volts = ((float) result [0])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreExtremeDroit = 34 / volts - 5;
            volts = ((float) result [3])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreExtremeGauche = 34 / volts - 5;
            volts = ((float) result [4])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreGauche = 34 / volts - 5;
            volts = ((float) result [2])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreCentre = 34 / volts - 5;
            volts = ((float) result [1])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreDroit = 34 / volts - 5;


            if (robotState.distanceTelemetreExtremeDroit > 30) {
                LED_ORANGE = 1;
            } else {
                LED_ORANGE = 0;
            }
            if (robotState.distanceTelemetreExtremeGauche > 30) {
                LED_BLANCHE = 1;
            } else {
                LED_BLANCHE = 0;
            }
//            if (robotState.distanceTelemetreCentre > 30) {
//                LED_BLEUE = 1;
//            } else {
//                LED_BLEUE = 0;
//            }
        }
    }

} // fin main

unsigned char stateRobot;

void OperatingSystemLoop(void)
{
    int vitesseD, vitesseG ;
    switch (stateRobot) {
        case STATE_ATTENTE:
            timestamp = 0;
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_ATTENTE_EN_COURS;
        case STATE_ATTENTE_EN_COURS:
            if (timestamp > 1000)
                stateRobot = STATE_AVANCE;
            break;
        case STATE_AVANCE:
            PWMSetSpeedConsigne(25, MOTEUR_DROIT);
            PWMSetSpeedConsigne(25, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE_EN_COURS;
            break;
        case STATE_AVANCE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_GAUCHE:
            PWMSetSpeedConsigne(15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_DROITE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE:
            PWMSetSpeedConsigne(15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE:
            PWMSetSpeedConsigne(-15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_RALENTIR:
            vitesseD = max (0.5 * robotState.distanceTelemetreGauche - 10 , 20) ;
            vitesseG = max(0.5 * robotState.distanceTelemetreDroit - 10, 20) ;
            PWMSetSpeedConsigne(vitesseD, MOTEUR_DROIT);
            PWMSetSpeedConsigne(vitesseG, MOTEUR_GAUCHE);
            stateRobot = STATE_RALENTIR_EN_COURS;
            break;
        case STATE_RALENTIR_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        case STATE_RALENTIR_D:
            vitesseD = 0.5 *robotState.distanceTelemetreGauche + 12.33 ;
            vitesseG = 20 ;
            PWMSetSpeedConsigne(vitesseD, MOTEUR_DROIT);
            PWMSetSpeedConsigne(vitesseG, MOTEUR_GAUCHE);
            stateRobot = STATE_RALENTIR_D_EN_COURS;
            break;
        case STATE_RALENTIR_D_EN_COURS:
            SetNextRobotStateInAutomaticMode();
        case STATE_RALENTIR_G:
            vitesseD = 20 ;
            vitesseG = 2/3 *robotState.distanceTelemetreDroit + 12.33 ;
            PWMSetSpeedConsigne(vitesseD, MOTEUR_DROIT);
            PWMSetSpeedConsigne(vitesseG, MOTEUR_GAUCHE);
            stateRobot = STATE_RALENTIR_G_EN_COURS;
            break;
        case STATE_RALENTIR_G_EN_COURS:
            SetNextRobotStateInAutomaticMode();
        default:
            stateRobot = STATE_ATTENTE;
            break;
    }
}
unsigned char nextStateRobot = 0;

void SetNextRobotStateInAutomaticMode()
{
    unsigned char positionObstacle = PAS_D_OBSTACLE ;
    //�Dtermination de la position des obstacles en fonction des ���tlmtres
    if ((robotState.distanceTelemetreExtremeDroit < 20 ||
            robotState.distanceTelemetreDroit < 30) &&
            robotState.distanceTelemetreCentre > 30 &&
            robotState.distanceTelemetreGauche > 30 &&
            robotState.distanceTelemetreExtremeGauche > 20) //Obstacle � droite
        positionObstacle = OBSTACLE_A_DROITE;
    else if (robotState.distanceTelemetreExtremeDroit > 20 &&
            robotState.distanceTelemetreDroit > 30 &&
            robotState.distanceTelemetreCentre > 30 &&
            (robotState.distanceTelemetreGauche < 30 ||
            robotState.distanceTelemetreExtremeGauche < 20)) //Obstacle � gauche
        positionObstacle = OBSTACLE_A_GAUCHE;
    else if (robotState.distanceTelemetreCentre < 30)
            positionObstacle = OBSTACLE_EN_FACE;
    else if (robotState.distanceTelemetreDroit < 45 || robotState.distanceTelemetreGauche < 45){
            positionObstacle = BIENTOT ;
        }
//    else if (robotState.distanceTelemetreDroit < 45 && robotState.distanceTelemetreGauche > 45){
//            positionObstacle = BIENTOT_D ;
//        }
//    else if (robotState.distanceTelemetreDroit > 45 && robotState.distanceTelemetreGauche < 45){
//            positionObstacle = BIENTOT_G ;
//        }
    else if (robotState.distanceTelemetreDroit > 45 &&
            robotState.distanceTelemetreCentre > 45 &&
            robotState.distanceTelemetreGauche > 45) //pas d?obstacle
        positionObstacle = PAS_D_OBSTACLE;
    
    
    
    //D�termination de l�?tat �venir du robot
    if (positionObstacle == PAS_D_OBSTACLE)
        nextStateRobot = STATE_AVANCE;
    else if (positionObstacle == OBSTACLE_A_DROITE)
        nextStateRobot = STATE_TOURNE_GAUCHE;
    else if (positionObstacle == OBSTACLE_A_GAUCHE)
        nextStateRobot = STATE_TOURNE_DROITE;
    else if (positionObstacle == BIENTOT)
        nextStateRobot = STATE_RALENTIR;
//    else if (positionObstacle == BIENTOT_D)
//        nextStateRobot = STATE_RALENTIR_G;
//    else if (positionObstacle == BIENTOT_G)
//        nextStateRobot = STATE_RALENTIR_D;
    else if (positionObstacle == OBSTACLE_EN_FACE){
        if(robotState.distanceTelemetreExtremeGauche > robotState.distanceTelemetreExtremeGauche)
        {
            nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
        }
        else
        {
            nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE ;
        }
    }
        
    //Si l?on n?est pas dans la transition de l�?tape en cours
    if (nextStateRobot != stateRobot - 1)
        stateRobot = nextStateRobot;
}

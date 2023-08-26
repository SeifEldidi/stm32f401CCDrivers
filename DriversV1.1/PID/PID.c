/*
 * PID.c
 *
 *  Created on: Aug 26, 2023
 *      Author: Seif pc
 */
#include "PID.h"

void PID_Tune(PID_CALC_t *PID_obj,float Kd,float Kp,float Ki,float DeltaT,float *OUT,float *In,float *Target)
{
	PID_obj->DeltaT   = DeltaT;
	PID_obj->Kd       = Kd;
	PID_obj->Ki 	  = Ki;
	PID_obj->Kp 	  = Kp;
	PID_obj->ERR 	  = 0;
	PID_obj->Prev_ERR = 0;
	PID_obj->I		  = 0;
	PID_obj->Input    = In;
	PID_obj->Output   = OUT;
	PID_obj->Target   = Target;
}


void PID_Compute(PID_CALC_t *PID_obj)
{
	float D = 0;
	float P = 0;
	/*--------Compute Error -------*/
	PID_obj->Prev_ERR = PID_obj->ERR;
	PID_obj->ERR = *PID_obj->Target - *PID_obj->Input;
	/*--------Compute Control Signal Components------*/
	P = PID_obj->Kp *PID_obj->ERR;
	D = PID_obj->Kd *( (PID_obj->ERR - PID_obj->Prev_ERR)/PID_obj->DeltaT);
	PID_obj->I	+=  PID_obj->Ki *( (PID_obj->ERR + PID_obj->Prev_ERR)/2)*PID_obj->DeltaT;
	/*--------Control Signal Calculation----*/
	*PID_obj->Output = D+ PID_obj->I + P;
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



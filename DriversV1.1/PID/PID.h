/*
 * PID.h
 *
 *  Created on: Aug 26, 2023
 *      Author: Seif pc
 */

#ifndef PID_H_
#define PID_H_

/*-------------Includes Section Start---------------*/
/*-------------Includes Section End----------------*/

/*------------Macro Declarations Start-----------*/

/*------------Macro Declarations End-----------*/

/*-----------Function Macro Start----------------*/

/*-----------Function Macro End----------------*/

/*----------Defined Data types start-------------------*/

typedef struct
{
	float DeltaT;
	float Kp;
	float Kd;
	float Ki;
	float Prev_ERR;
	float ERR;
	float I ;
	float *Input ,*Output ,*Target;
}PID_CALC_t;
/*----------Defined Data types End----------------*/

/*-----------Software Interfaces start--------------*/
long map(long x, long in_min, long in_max, long out_min, long out_max);
void PID_Tune(PID_CALC_t *PID_obj,float Kd,float Kp,float Ki,float DeltaT,float *OUT,float *In,float *Target);
void PID_Compute(PID_CALC_t *PID_obj);
/*-----------Software Interfaces End--------------*/

#endif /* PID_H_ */

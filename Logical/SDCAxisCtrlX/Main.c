
#include <bur/plctypes.h>

#ifdef _DEFAULT_INCLUDES
	#include <AsDefault.h>
#endif

void _INIT ProgramInit(void)
{
	axis_X.dt = 0.002f;
	
	Axis_X_HW.EncIf1_Typ = ncSDC_ENC16;
	Axis_X_HW.DiDoIf_Typ = ncSDC_DIDO;
	Axis_X_HW.DrvIf_Typ = ncSDC_DRVSERVO16;
	
	strcpy(Axis_X_HW.EncIf1_Name, "Axis_X_EncIf");
	strcpy(Axis_X_HW.DrvIf_Name, "Axis_X_DrvIf");
	strcpy(Axis_X_HW.DiDoIf_Name, "Axis_X_DiDoIf");
	Axis_X_EncIf.iEncOK = 1;
	Axis_X_DrvIf.iDrvOK = 1;
	Axis_X_DrvIf.iStatusEnable = 1;
	Axis_X_DiDoIf.iDriveReady = 1;
	
	fb_regulator.dt = 0.002f;
	//fb_motor.dt = 0.01f;
	//fb_motor.ke = 0.016f;
	//fb_motor.Tm = 0.1f;
	fb_regulator.max_abs_value = 24;
	fb_regulator.k_i = 0.16f;
	fb_regulator.k_p = 0.0024f;
	fb_regulator.iyOld = 0.0f;
	
	pwm_period = 200;
}

void _CYCLIC ProgramCyclic(void)
{
	Axis_X_EncIf.iLifeCnt++;
	Axis_X_DiDoIf.iLifeCntDriveEnable++;
	Axis_X_DiDoIf.iLifeCntDriveReady++;
	Axis_X_DiDoIf.iLifeCntNegHwEnd++;
	Axis_X_DiDoIf.iLifeCntPosHwEnd++;
	Axis_X_DiDoIf.iLifeCntReference++;
	Axis_X_DrvIf.iLifeCnt++;
	Axis_X_EncIf.iActTime = (INT)AsIOTimeCyclicStart();
	
	Axis_X_EncIf.iActPos = axis_X.counter;
	Axis_X_DiDoIf.iPosHwEnd = axis_X.endswitch_a_reached;
	Axis_X_DiDoIf.iNegHwEnd = axis_X.endswitch_b_reached;
	
	coil_pwm_value = coil_powered ? 32767 : 0;
	
	fb_regulator.e = Axis_X_DrvIf.oSetPos * 6500.0f / 32767.0f - axis_X.speed;
	FB_Regulator(&fb_regulator);
	axis_X.u = fb_regulator.u;
	
	if (pos_hand && !axis_X.endswitch_a_reached)
		axis_X.u = 6.0f;
	else if (neg_hand && !axis_X.endswitch_b_reached)
		axis_X.u = -6.0f;
	
	FB_Axis(&axis_X);
	
}

void _EXIT ProgramExit(void)
{

}


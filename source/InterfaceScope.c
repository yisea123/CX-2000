// InterfaceScope.c

//#define TEST_DEVICE_B4  /* 测试设备编号 */




#ifndef    __INTERFACE_SCOPE_C__
#define    __INTERFACE_SCOPE_C__

#include "KernelHeader.h"


//-----------------------------------------------------------------------------------------

//--------------------------------
// output computing, attention: 0-on, 1-off
_STA_ IO_ UINT16 XRAM_ m_nIoValves = 0xFFFF; // 01 ~ 16

// for network testing
_STA_ IO_ UINT16 XRAM_ s_anBufNet[1500];
_STA_ IO_ UINT16 XRAM_ s_nDataLen = 0;
_STA_ IO_ UINT16 XRAM_ s_nStatus  = 0;

#ifdef  TEST_DEVICE_B4    /* 设备B4专用参数 */
_STA_ UINT32 s_nK = 1543600;
_STA_ UINT32 s_nB = 1131880000;
#else                    /* 通用参数 */
_STA_ UINT32 s_nK = 173280;
_STA_ UINT32 s_nB = 97380000;
#endif

//-----------------------------------------------------------------------------------------

// init
UINT8 MT_ALL_Init(void)
{
    // axis x
#if 0
    MV_InitPara(Motor_X, 5000, AXIS_X_SPEED_TOP, 100, 10);
#endif
    MV_InitPara(Motor_X,
                4000,
                8000,
                50,
                1);
    // axis y
    MV_InitPara(Motor_Y, 4000, 8000, 100, 10);

    MV_Stop(Motor_X);
    MV_Stop(Motor_Y);

    return e_Feedback_Success;
}

// axis x
enum eAxisXPos MT_X_get_posi(void)
{
    if (E_AXIS_X_POS_UNSURE == g_tAxisPosStatus.eAxisX)
    {
        if (0 == HW_LEVEL_GetOC(OC_HOME_CHANNEL))
        {
            g_tAxisPosStatus.eAxisX = E_AXIS_X_POS_HOME;
        }
        else if (0 == HW_LEVEL_GetOC(OC_OUT_CHANNEL))
        {
            g_tAxisPosStatus.eAxisX = E_AXIS_X_POS_CTRL;
        }
    }
    return g_tAxisPosStatus.eAxisX;
}

enum eAxisYPos MT_Y_get_posi(void)
{
    if (E_AXIS_Y_POS_UNSURE == g_tAxisPosStatus.eAxisY)
    {
        if (0 == HW_LEVEL_GetOC(OC_SAMPLE_RELEA_CHANNEL))
        {
            g_tAxisPosStatus.eAxisY = E_AXIS_Y_POS_HOME;
        }
    }

    return g_tAxisPosStatus.eAxisY;
}


//UINT8 MT_X_Home(void)
//{
//    struct tMvMotorPara  tMvoingPara;

//#ifdef  MOTO_DOBULE_ENABLE
//    MT_Y_MoveToPosRel();
//#endif

//    moto_work_stat(0, MOTO_WORK_STAT_RUN);  /* 动作开始执行 */

//    // record the motor's para
//    tMvoingPara.nFreqMin = g_atMotorPara[Motor_X].nFreqMin;
//    tMvoingPara.nFreqMax = g_atMotorPara[Motor_X].nFreqMax;
//    tMvoingPara.nFreqInc = g_atMotorPara[Motor_X].nFreqInc;
//    tMvoingPara.nFreqSam = g_atMotorPara[Motor_X].nFreqSam;

//    // not detect the single of home at the begining, moving long diatance
//    if (1 == HW_LEVEL_GetOC(OC_HOME_CHANNEL))
//    {
//			  /* ******************** original code *********************
//        // long distance
//			  MV_InitPara(Motor_X, 500, 12000, 2000, 100);    // ZZC_0904   : MV_InitPara(Motor_X, 4000, 8000, 100, 10);
//        // OC is on the right, right step
//        MV_Move(Motor_X, 35000, e_Dir_Neg); // comes near the OC

//        while (1 == HW_LEVEL_GetOC(OC_HOME_CHANNEL)) // OC detection is not enable
//        {
//            if (e_True == MV_IsFinished(Motor_X))
//            {
//                MV_Stop(Motor_X);
//                collect_return_hdl(COLLECT_RET_FAIL_SAMPLE);

//                moto_work_stat(0, MOTO_WORK_STAT_FAIL);  // 动作执行失败
//                return e_Feedback_Error;
//            }
//        }
//        IT_SYS_DlyMs(3);      //
//        MV_Stop(Motor_X); 
//				****************************************************** */
//		// long distance
//		MV_InitPara(Motor_X, 1920, 10000, 100, 10);    // ZZC_0904   : MV_InitPara(Motor_X, 4000, 8000, 100, 10);
//        // OC is on the right, right step
//        MV_Move(Motor_X, 30000, e_Dir_Neg); // comes near the OC

//        /*while (1 == HW_LEVEL_GetOC(OC_HOME_CHANNEL)) // OC detection is not enable
//        {
//            if (e_True == MV_IsFinished(Motor_X))
//            {
//                MV_Stop(Motor_X);
//                collect_return_hdl(COLLECT_RET_FAIL_SAMPLE);

//                moto_work_stat(0, MOTO_WORK_STAT_FAIL);  // 动作执行失败
//                return e_Feedback_Error;
//            }
//        }*/
//		while (1 == HW_LEVEL_GetOC(OC_HOME_CHANNEL))  //&& (0 == MV_IsFinished(Motor_X))
//		{
//            if (e_True == MV_IsFinished(Motor_X))
//            {
//                MV_Stop(Motor_X);
//                break;
//            }
//        }
//		MV_InitPara(Motor_X, 1920, 1920, 100, 10);    // ZZC_0904   : MV_InitPara(Motor_X, 4000, 8000, 100, 10);
//        // OC is on the right, right step
//        MV_Move(Motor_X, 5000, e_Dir_Neg); // comes near the OC
//		while (1 == HW_LEVEL_GetOC(OC_HOME_CHANNEL))  //&& (0 == MV_IsFinished(Motor_X))
//		{
//            if (e_True == MV_IsFinished(Motor_X))
//            {
//                MV_Stop(Motor_X);
//                collect_return_hdl(COLLECT_RET_FAIL_SAMPLE);

//                moto_work_stat(0, MOTO_WORK_STAT_FAIL);  // 动作执行失败
//                return e_Feedback_Error;
//            }
//        }
//        //IT_SYS_DlyMs(1);      //
//				//MV_InitPara(Motor_X, 1920, 1920, 100, 10);    // ZZC_0904   : MV_InitPara(Motor_X, 4000, 8000, 100, 10);
//        // OC is on the right, right step
//        //MV_Move(Motor_X, 500, e_Dir_Neg); // comes near the OC
//				//while (1)  //&& (0 == MV_IsFinished(Motor_X))
//				//{
//        //    if (e_True == MV_IsFinished(Motor_X))
//        //    {
//        //        MV_Stop(Motor_X);
//        //        break;
//        //    }
//        //}
//        MV_Stop(Motor_X); 
//    }
//    else
//    {
//#if  0    /* 样本仓到位，不再运行 */
//        //------------------------------------------------------------------
//        // short distance, home detect
//        MV_InitPara(Motor_X, 2000, 3000, 100, 1);
//        //------------------------------------------
//        // OC is on the right, left step
//        MV_Move(Motor_X, 2000, e_Dir_Pos); // goes far away the OC
//        while (0 == HW_LEVEL_GetOC(OC_HOME_CHANNEL))
//        {
//            if (e_True == MV_IsFinished(Motor_X))
//            {
//                MV_Stop(Motor_X);
//                return e_Feedback_Error;
//            }
//        }
//        IT_SYS_DlyMs(3);      //
//        MV_Stop(Motor_X);
//#endif
//    }
//    //
//    g_tAxisPosStatus.nAxisX = 0;
//    g_tAxisPosStatus.eAxisX    = E_AXIS_X_POS_HOME;
//    //---------------------------------------------------
//    // set to the default moving parameters
//    MV_InitPara(Motor_X,
//                tMvoingPara.nFreqMin,
//                tMvoingPara.nFreqMax,
//                tMvoingPara.nFreqInc,
//                tMvoingPara.nFreqSam);
//    //
//    moto_work_stat(0, MOTO_WORK_STAT_OK);  /* 动作执行成功 */
//    return e_Feedback_Success;
//}


UINT8 MT_X_Home(CALL_STYLE_E eCall) // flag=0: normal check callback, =1: self check callback
{
    struct tMvMotorPara  tMvoingPara;
	UINT32 nStep = 0;
//	char buf[30] = {0}; UINT8 OC;

	if(1 == HW_LEVEL_GetOC(OC_HOME_CHANNEL)) 
	{
#ifdef  MOTO_DOBULE_ENABLE
		MT_Y_MoveToPosRel(eCall);
#endif
	}else{ // not need move, but the status msg need feekback to app
		if(eCall == e_NormalCheck_Call)
		{
			moto_work_stat_2(1, MOTO_WORK_STAT_RUN, e_BUILD_PRESS_SUCCESS);
			moto_work_stat_2(1, MOTO_WORK_STAT_OK, e_BUILD_PRESS_SUCCESS);
		}
	}
	
	if(eCall == e_NormalCheck_Call)
	{
		//moto_work_stat(0, MOTO_WORK_STAT_RUN);  /* 动作开始执行 */
		moto_work_stat_2(0, MOTO_WORK_STAT_RUN, e_BUILD_PRESS_SUCCESS);  /* 动作开始执行 */
	}
    // record the motor's para
    tMvoingPara.nFreqMin = g_atMotorPara[Motor_X].nFreqMin;
    tMvoingPara.nFreqMax = g_atMotorPara[Motor_X].nFreqMax;
    tMvoingPara.nFreqInc = g_atMotorPara[Motor_X].nFreqInc;
    tMvoingPara.nFreqSam = g_atMotorPara[Motor_X].nFreqSam;

    // not detect the single of home at the begining, moving long diatance
    if (1 == HW_LEVEL_GetOC(OC_HOME_CHANNEL))
    {
		// long distance
		MV_InitPara(Motor_X, 1920, 10000, 100, 10);    // ZZC_0904   : MV_InitPara(Motor_X, 4000, 8000, 100, 10);
        // OC is on the right, right step
        MV_Move(Motor_X, 30000, e_Dir_Neg); // comes near the OC
		while (1 == HW_LEVEL_GetOC(OC_HOME_CHANNEL))  //&& (0 == MV_IsFinished(Motor_X))
		{			
            if (e_True == MV_IsFinished(Motor_X))
            {
                MV_Stop(Motor_X);
                break;
            }
        }
		MV_InitPara(Motor_X, 1920, 1920, 100, 10);    // ZZC_0904   : MV_InitPara(Motor_X, 4000, 8000, 100, 10);
        // OC is on the right, right step
        MV_Move(Motor_X, 5000, e_Dir_Neg); // comes near the OC
		while (1 == HW_LEVEL_GetOC(OC_HOME_CHANNEL))  //&& (0 == MV_IsFinished(Motor_X))
		{
            if (e_True == MV_IsFinished(Motor_X))
            {
                MV_Stop(Motor_X);
				if(eCall == e_NormalCheck_Call)
				{
					collect_return_hdl(COLLECT_RET_FAIL_SAMPLE);
					//moto_work_stat(0, MOTO_WORK_STAT_FAIL);  // 动作执行失败					
					moto_work_stat_2(0, MOTO_WORK_STAT_FAIL, e_BUILD_PRESS_SUCCESS);
				}
                return e_Feedback_Error;
            }
        }
        MV_Stop(Motor_X); 
		
#if 1 // add step 
//		IT_SYS_DlyMs(1000);
//		IT_SYS_DlyMs(1000);
		if(g_Record_Param.nXAddStep > 0)
		{
			MV_InitPara(Motor_X, 1920, 1920, 100, 10);
			MV_Move(Motor_X, (UINT32)g_Record_Param.nXAddStep, e_Dir_Neg); // comes near the OC 
			while(1)
			{
				MV_GetStepsExecuted(Motor_X, &nStep);
				if(g_Record_Param.nXAddStep - nStep <= 5)
				{
					MV_Stop(Motor_X);
					break;
				}
				IT_SYS_DlyMs(2);
			}
		}
		printf("Moto X Add Step(%d) Finshed\r\n", (int)g_Record_Param.nXAddStep);
#endif
		MV_Stop(Motor_X); 
		
		
    }
    else
    {
#if  0    /* 样本仓到位，不再运行 */
        //------------------------------------------------------------------
        // short distance, home detect
        MV_InitPara(Motor_X, 2000, 3000, 100, 1);
        //------------------------------------------
        // OC is on the right, left step
        MV_Move(Motor_X, 2000, e_Dir_Pos); // goes far away the OC
        while (0 == HW_LEVEL_GetOC(OC_HOME_CHANNEL))
        {
            if (e_True == MV_IsFinished(Motor_X))
            {
                MV_Stop(Motor_X);
                return e_Feedback_Error;
            }
        }
        IT_SYS_DlyMs(3);      //
        MV_Stop(Motor_X);
#endif
    }
    //
    g_tAxisPosStatus.nAxisX = 0;
    g_tAxisPosStatus.eAxisX    = E_AXIS_X_POS_HOME;
    //---------------------------------------------------
    // set to the default moving parameters
    MV_InitPara(Motor_X,
                tMvoingPara.nFreqMin,
                tMvoingPara.nFreqMax,
                tMvoingPara.nFreqInc,
                tMvoingPara.nFreqSam);
    //
	if(eCall == e_NormalCheck_Call)
	{
		//moto_work_stat(0, MOTO_WORK_STAT_OK);  /* 动作执行成功 */
		moto_work_stat_2(0, MOTO_WORK_STAT_OK, e_BUILD_PRESS_SUCCESS);
	}
    return e_Feedback_Success;
}


UINT8 MT_X_Home_only(void)
{
    struct tMvMotorPara  tMvoingPara;

    //moto_work_stat(0, MOTO_WORK_STAT_RUN);  /* 动作开始执行 */
	moto_work_stat_2(0, MOTO_WORK_STAT_RUN, e_BUILD_PRESS_SUCCESS);

    // record the motor's para
    tMvoingPara.nFreqMin = g_atMotorPara[Motor_X].nFreqMin;
    tMvoingPara.nFreqMax = g_atMotorPara[Motor_X].nFreqMax;
    tMvoingPara.nFreqInc = g_atMotorPara[Motor_X].nFreqInc;
    tMvoingPara.nFreqSam = g_atMotorPara[Motor_X].nFreqSam;

    // not detect the single of home at the begining, moving long diatance
    if (1 == HW_LEVEL_GetOC(OC_HOME_CHANNEL))
    {
        // long distance
        MV_InitPara(Motor_X, 4000, 8000, 100, 10);
        // OC is on the right, right step
        MV_Move(Motor_X, 35000, e_Dir_Neg); // comes near the OC

        while (1 == HW_LEVEL_GetOC(OC_HOME_CHANNEL)) // OC detection is not enable
        {
            if (e_True == MV_IsFinished(Motor_X))
            {
                MV_Stop(Motor_X);
                collect_return_hdl(COLLECT_RET_FAIL_SAMPLE);

                //moto_work_stat(0, MOTO_WORK_STAT_FAIL);  /* 动作执行失败 */
				moto_work_stat_2(0, MOTO_WORK_STAT_FAIL, e_BUILD_PRESS_SUCCESS);
                return e_Feedback_Error;
            }
        }
        IT_SYS_DlyMs(3);      //
        MV_Stop(Motor_X);
    }
    else
    {
        ;
    }
    //
    g_tAxisPosStatus.nAxisX = 0;
    g_tAxisPosStatus.eAxisX    = E_AXIS_X_POS_HOME;
    //---------------------------------------------------
    // set to the default moving parameters
    MV_InitPara(Motor_X,
                tMvoingPara.nFreqMin,
                tMvoingPara.nFreqMax,
                tMvoingPara.nFreqInc,
                tMvoingPara.nFreqSam);
    //
    //moto_work_stat(0, MOTO_WORK_STAT_OK);  /* 动作执行成功 */
	moto_work_stat_2(0, MOTO_WORK_STAT_OK, e_BUILD_PRESS_SUCCESS);
    return e_Feedback_Success;
}

#ifdef  MOTO_Y_VERSION_V4
/* 4.0版本结构 */
UINT8 MT_Y_Home(CALL_STYLE_E eCall)
{
	if(eCall == e_NormalCheck_Call && E_AXIS_Y_POS_UNSURE != g_tAxisPosStatus.eAxisY)
	{
		//moto_work_stat(1, MOTO_WORK_STAT_RUN);
		moto_work_stat_2(1, MOTO_WORK_STAT_RUN, e_BUILD_PRESS_SUCCESS);
	}
    if (E_AXIS_Y_POS_UNSURE == g_tAxisPosStatus.eAxisY)
    {
        MT_Y_MoveToPosRel(eCall);
    }
    //
    if (E_AXIS_Y_POS_HOME == g_tAxisPosStatus.eAxisY)
    {
        MV_Move(Motor_Y, 2000, e_Dir_Pos);
        //
        g_tAxisPosStatus.eAxisY = E_AXIS_Y_POS_CTRL;
    }
    //

	if(eCall == e_NormalCheck_Call && E_AXIS_Y_POS_UNSURE != g_tAxisPosStatus.eAxisY)
	{
		//moto_work_stat(1, MOTO_WORK_STAT_OK);
		moto_work_stat_2(1, MOTO_WORK_STAT_OK, e_BUILD_PRESS_SUCCESS);
	}
    return e_Feedback_Success;
}

#else

UINT8 MT_Y_Home(void)
{
    struct tMvMotorPara  tMvoingPara;

    //moto_work_stat(1, MOTO_WORK_STAT_RUN);  /* 动作开始执行 */
	moto_work_stat_2(1, MOTO_WORK_STAT_RUN, e_BUILD_PRESS_SUCCESS);

    // record the motor's para
    tMvoingPara.nFreqMin = g_atMotorPara[Motor_Y].nFreqMin;
    tMvoingPara.nFreqMax = g_atMotorPara[Motor_Y].nFreqMax;
    tMvoingPara.nFreqInc = g_atMotorPara[Motor_Y].nFreqInc;
    tMvoingPara.nFreqSam = g_atMotorPara[Motor_Y].nFreqSam;

    // not detect the single of home at the begining, moving long diatance
    if (1 == HW_LEVEL_GetOC(OC_SAMPLE_RELEA_CHANNEL))
    {
        // long distance
        MV_InitPara(Motor_Y, 4000, 8000, 100, 10);
        // OC is on the right, right step
        MV_Move(Motor_Y, 35000, e_Dir_Pos); // comes near the OC

        while (1 == HW_LEVEL_GetOC(OC_SAMPLE_RELEA_CHANNEL)) // OC detection is not enable
        {
            if (e_True == MV_IsFinished(Motor_Y))
            {
                MV_Stop(Motor_Y);
                collect_return_hdl(COLLECT_RET_FAIL_SAMPLE);

               //moto_work_stat(1, MOTO_WORK_STAT_FAIL);  /* 动作执行失败 */
				moto_work_stat_2(1, MOTO_WORK_STAT_FAIL, e_BUILD_PRESS_SUCCESS);
                return e_Feedback_Error;
            }
        }
        IT_SYS_DlyMs(3);      //
        MV_Stop(Motor_Y);
    }
    else
    {
        ;
    }
    //
    g_tAxisPosStatus.nAxisY = 0;
    g_tAxisPosStatus.eAxisY = E_AXIS_Y_POS_HOME;
    //---------------------------------------------------
    // set to the default moving parameters
    MV_InitPara(Motor_Y,
                tMvoingPara.nFreqMin,
                tMvoingPara.nFreqMax,
                tMvoingPara.nFreqInc,
                tMvoingPara.nFreqSam);
    //
    //moto_work_stat(1, MOTO_WORK_STAT_OK);  /* 动作执行成功 */
	moto_work_stat_2(1, MOTO_WORK_STAT_OK, e_BUILD_PRESS_SUCCESS);
    return e_Feedback_Success;
}
#endif

UINT8 MT_Y_MoveToPosRel_Self_Check(void)
{
	struct tMvMotorPara  tMvoingPara;

//    moto_work_stat(1, MOTO_WORK_STAT_RUN);  /* 动作开始执行 */

    // record the motor's para
    tMvoingPara.nFreqMin = g_atMotorPara[Motor_Y].nFreqMin;
    tMvoingPara.nFreqMax = g_atMotorPara[Motor_Y].nFreqMax;
    tMvoingPara.nFreqInc = g_atMotorPara[Motor_Y].nFreqInc;
    tMvoingPara.nFreqSam = g_atMotorPara[Motor_Y].nFreqSam;

    // not detect the single of home at the begining, moving long diatance
    if (1 == HW_LEVEL_GetOC(OC_SAMPLE_RELEA_CHANNEL))
    {
        // long distance
        MV_InitPara(Motor_Y, 4000, 8000, 100, 10);
        // OC is on the right, right step
        MV_Move(Motor_Y, 35000, e_Dir_Neg); // comes far the OC

        while (1 == HW_LEVEL_GetOC(OC_SAMPLE_RELEA_CHANNEL)) // OC detection is not enable
        {
            if (e_True == MV_IsFinished(Motor_Y))
            {
                MV_Stop(Motor_Y);
 //               collect_return_hdl(COLLECT_RET_FAIL_SAMPLE);

 //               moto_work_stat(1, MOTO_WORK_STAT_FAIL);  /* 动作执行失败 */
                return e_Feedback_Error;
            }
        }

        /* 增加临时运行时序 */
        MV_Move(Motor_Y, 600, e_Dir_Neg);  /* 继续运行 */
        while (e_False == MV_IsFinished(Motor_Y));
        /* 增加临时运行时序 */

        IT_SYS_DlyMs(3);
        MV_Stop(Motor_Y);
    }
    else
    {
        /* 增加临时运行时序 */
        MV_InitPara(Motor_Y, 4000, 8000, 100, 10);
        MV_Move(Motor_Y, 20000, e_Dir_Pos);

        while (0 == HW_LEVEL_GetOC(OC_SAMPLE_RELEA_CHANNEL)) // OC detection is not enable
        {
            if (e_True == MV_IsFinished(Motor_Y))
            {
                MV_Stop(Motor_Y);

 //               collect_return_hdl(COLLECT_RET_FAIL_SAMPLE);
 //               moto_work_stat(1, MOTO_WORK_STAT_FAIL);  /* 动作执行失败 */
                return e_Feedback_Error;
            }
        }

        MV_Move(Motor_Y, 600, e_Dir_Neg);  /* 继续运行 */
        while (e_False == MV_IsFinished(Motor_Y));

        IT_SYS_DlyMs(3);
        MV_Stop(Motor_Y);
        /* 增加临时运行时序 */
    }

    //
    g_tAxisPosStatus.nAxisY = 0;
    g_tAxisPosStatus.eAxisY = E_AXIS_Y_POS_HOME;
    //---------------------------------------------------
    // set to the default moving parameters
    MV_InitPara(Motor_Y,
                tMvoingPara.nFreqMin,
                tMvoingPara.nFreqMax,
                tMvoingPara.nFreqInc,
                tMvoingPara.nFreqSam);
    //
//    moto_work_stat(1, MOTO_WORK_STAT_OK);  /* 动作执行成功 */
    return e_Feedback_Success;

}

_EXT_ UINT8 MT_Y_Home_Self_Check(void)
{
  // moto_work_stat(1, MOTO_WORK_STAT_RUN);
    if (E_AXIS_Y_POS_UNSURE == g_tAxisPosStatus.eAxisY)
    {
        MT_Y_MoveToPosRel_Self_Check();
    }
    //
    if (E_AXIS_Y_POS_HOME == g_tAxisPosStatus.eAxisY)
    {
        MV_Move(Motor_Y, 2000, e_Dir_Pos);
        //
        g_tAxisPosStatus.eAxisY = E_AXIS_Y_POS_CTRL;
    }
    //

  //  moto_work_stat(1, MOTO_WORK_STAT_OK);
    return e_Feedback_Success;
}

#if  0    /* 增加出仓光耦检测，所以修改该函数 */
UINT8 MT_X_MoveToPosRel(void)
{
    // MV_InitPara(Motor_X, 4000, 8000, 100, 10);
    //
    if (E_AXIS_X_POS_UNSURE == g_tAxisPosStatus.eAxisX)
    {
        MT_X_Home();
    }
    //
    if (E_AXIS_X_POS_HOME == g_tAxisPosStatus.eAxisX)
    {
        MV_Move(Motor_X, 30000, e_Dir_Pos);
        //
        g_tAxisPosStatus.eAxisX = E_AXIS_X_POS_CTRL;
    }
    //
    return e_Feedback_Success;
}
#else

UINT8 MT_X_MoveToPosRel(CALL_STYLE_E eCall)
{
    struct tMvMotorPara  tMvoingPara;
	INT32 nPress = 0;
	
	UINT32 StartTicks = 0, EndTicks = 0;
//	UINT8 i = 0;
	IO_ UINT8 flag = 0;
	
	if(eCall == e_NormalCheck_Call)
	{
		//moto_work_stat(0, MOTO_WORK_STAT_RUN);  /* 动作开始执行 */
		moto_work_stat_2(0, MOTO_WORK_STAT_RUN, e_BUILD_PRESS_SUCCESS);
		nPress = Get_Press_Value(GET_PRESS_NUM_FIVE);
		printf("\r\n1 X out-check press at first: npress=%010d, addpress=%010d\r\n", (int)nPress, (int)g_Record_Param.nAddPress);
		//StartTicks = IT_SYS_GetTicks();
		if(nPress < PRESS_BUILD)
		{
			HW_PUMP_Pulse(PUMP_PRESS_FREQ, e_Dir_Pos);
			HW_Valve_On(INDEX_VALVE_PUMP);
			HW_Valve_Off(INDEX_VALVE_WBC);			
		}
	}
    // record the motor's para
    tMvoingPara.nFreqMin = g_atMotorPara[Motor_X].nFreqMin;
    tMvoingPara.nFreqMax = g_atMotorPara[Motor_X].nFreqMax;
    tMvoingPara.nFreqInc = g_atMotorPara[Motor_X].nFreqInc;
    tMvoingPara.nFreqSam = g_atMotorPara[Motor_X].nFreqSam;
	
    // not detect the single of home at the begining, moving long diatance
    if (1 == HW_LEVEL_GetOC(OC_OUT_CHANNEL))
    {
        // long distance
		MV_InitPara(Motor_X, 3000, 10000, 100, 10);   // ZZC_0904 : MV_InitPara(Motor_X, 4000, 8000, 100, 10); 
        // OC is on the right, right step
        MV_Move(Motor_X, 35000, e_Dir_Pos);  /* 靠近出仓光耦方向 */

        while(1 == HW_LEVEL_GetOC(OC_OUT_CHANNEL)) // OC detection is not enable
        {
			if(eCall == e_NormalCheck_Call)
			{
				//nPress = HW_ADC_SpiGetPress();
				nPress = Get_Press_Value(GET_PRESS_NUM_FIVE);
				if(nPress >= PRESS_BUILD)
				{
					if(flag == 0 || flag == 1)
					{
						HW_PUMP_Pulse(PUMP_PRESS_OFF, e_Dir_Pos);
						HW_Valve_Off(INDEX_VALVE_PUMP);
						HW_Valve_Off(INDEX_VALVE_WBC);
						flag = 2;
					}
				}else{
					if(flag == 0)
					{
						HW_PUMP_Pulse(PUMP_PRESS_FREQ, e_Dir_Pos);
						HW_Valve_On(INDEX_VALVE_PUMP);
						HW_Valve_Off(INDEX_VALVE_WBC);
						flag = 1;
					}
				}
			}
			//
            if (e_True == MV_IsFinished(Motor_X))
            {
                MV_Stop(Motor_X);
				if(eCall == e_NormalCheck_Call)
				{
					collect_return_hdl(COLLECT_RET_FAIL_SAMPLE);
					//moto_work_stat(0, MOTO_WORK_STAT_FAIL);  /* 动作执行失败 */
					moto_work_stat_2(0, MOTO_WORK_STAT_FAIL, e_BUILD_PRESS_SUCCESS);
				}
                return e_Feedback_Error;
            }
		}
		printf("2 X out-build press over: npress=%010d, addpress=%010d\r\n", (int)nPress, (int)g_Record_Param.nAddPress);
		IT_SYS_DlyMs(3);      //
		MV_Stop(Motor_X);
    }
    else
    {
        ;  /* 样本仓到位，不再运行 */
    }
	// 
	flag = 0;
	if(eCall == e_NormalCheck_Call)
	{
		HW_PUMP_Pulse(PUMP_PRESS_OFF, e_Dir_Pos);
		HW_Valve_Off(INDEX_VALVE_PUMP);
		HW_Valve_Off(INDEX_VALVE_WBC);	
		IT_SYS_DlyMs(500);
	}
	if(eCall == e_NormalCheck_Call)
	{
		StartTicks = IT_SYS_GetTicks();
		EndTicks = StartTicks;
		while((EndTicks - StartTicks) <= TIME_OVER_TS_BUILD_PRESS)
		{
			EndTicks = IT_SYS_GetTicks();
			nPress = Get_Press_Value(GET_PRESS_NUM_FIVE);
			if(nPress >= PRESS_BUILD) 
			{
				HW_PUMP_Pulse(PUMP_PRESS_OFF, e_Dir_Pos);
				HW_Valve_Off(INDEX_VALVE_PUMP);
				HW_Valve_Off(INDEX_VALVE_WBC);	
				break;
			}else{
				if(flag == 0)
				{
					HW_PUMP_Pulse(PUMP_PRESS_FREQ, e_Dir_Pos);
					HW_Valve_On(INDEX_VALVE_PUMP);
					HW_Valve_Off(INDEX_VALVE_WBC);
					flag = 1;
				}
			}
			IT_SYS_DlyMs(6);
		}
		printf("3 X out check press after build: npress=%010d, addpress=%010d\r\n", (int)nPress, (int)g_Record_Param.nAddPress);
		HW_PUMP_Pulse(PUMP_PRESS_OFF, e_Dir_Pos);
		HW_Valve_Off(INDEX_VALVE_PUMP);
		HW_Valve_Off(INDEX_VALVE_WBC);	
	}
	//
    g_tAxisPosStatus.nAxisX = 0;
    g_tAxisPosStatus.eAxisX    = E_AXIS_X_POS_CTRL;
    //---------------------------------------------------
    // set to the default moving parameters
    MV_InitPara(Motor_X,
                tMvoingPara.nFreqMin,
                tMvoingPara.nFreqMax,
                tMvoingPara.nFreqInc,
                tMvoingPara.nFreqSam);
	//EndTicks = IT_SYS_GetTicks();
	if(eCall == e_NormalCheck_Call)
	{
		if((EndTicks - StartTicks) > TIME_OVER_TS_BUILD_PRESS)
		{
			moto_work_stat_2(0, MOTO_WORK_STAT_OK, e_BUILD_PRESS_FAIL);
			printf("4 X out at last,build press error: t=%08d, npress=%010d, addpress=%010d\r\n", (int)(EndTicks - StartTicks), (int)nPress, (int)g_Record_Param.nAddPress);
		}else{
			moto_work_stat_2(0, MOTO_WORK_STAT_OK, e_BUILD_PRESS_SUCCESS);
			printf("4 X out at last,build press success: t=%08d, npress=%010d, addpress=%010d\r\n", (int)(EndTicks - StartTicks), (int)nPress, (int)g_Record_Param.nAddPress);
		}
	}
#ifdef  MOTO_DOBULE_ENABLE
    MT_Y_Home(eCall);
#endif

    return e_Feedback_Success;
}


UINT8 MT_X_MoveToPosRel_only(void)
{
    struct tMvMotorPara  tMvoingPara;

   // moto_work_stat(0, MOTO_WORK_STAT_RUN);  /* 动作开始执行 */
	moto_work_stat_2(0, MOTO_WORK_STAT_RUN, e_BUILD_PRESS_SUCCESS);
    // record the motor's para
    tMvoingPara.nFreqMin = g_atMotorPara[Motor_X].nFreqMin;
    tMvoingPara.nFreqMax = g_atMotorPara[Motor_X].nFreqMax;
    tMvoingPara.nFreqInc = g_atMotorPara[Motor_X].nFreqInc;
    tMvoingPara.nFreqSam = g_atMotorPara[Motor_X].nFreqSam;

    // not detect the single of home at the begining, moving long diatance
    if (1 == HW_LEVEL_GetOC(OC_OUT_CHANNEL))
    {
        // long distance
        MV_InitPara(Motor_X, 4000, 8000, 100, 10);
        // OC is on the right, right step
        MV_Move(Motor_X, 35000, e_Dir_Pos);  /* 靠近出仓光耦方向 */

        while (1 == HW_LEVEL_GetOC(OC_OUT_CHANNEL)) // OC detection is not enable
        {
            if (e_True == MV_IsFinished(Motor_X))
            {
                MV_Stop(Motor_X);
                collect_return_hdl(COLLECT_RET_FAIL_SAMPLE);

                //moto_work_stat(0, MOTO_WORK_STAT_FAIL);  /* 动作执行失败 */
				moto_work_stat_2(0, MOTO_WORK_STAT_FAIL, e_BUILD_PRESS_SUCCESS);
                return e_Feedback_Error;
            }
        }
        IT_SYS_DlyMs(3);      //
        MV_Stop(Motor_X);
    }
    else
    {
        ;  /* 样本仓到位，不再运行 */
    }
    //
    g_tAxisPosStatus.nAxisX = 0;
    g_tAxisPosStatus.eAxisX    = E_AXIS_X_POS_CTRL;
    //---------------------------------------------------
    // set to the default moving parameters
    MV_InitPara(Motor_X,
                tMvoingPara.nFreqMin,
                tMvoingPara.nFreqMax,
                tMvoingPara.nFreqInc,
                tMvoingPara.nFreqSam);
    //
    //moto_work_stat(0, MOTO_WORK_STAT_OK);  /* 动作执行成功 */
	moto_work_stat_2(0, MOTO_WORK_STAT_OK, e_BUILD_PRESS_SUCCESS);

    return e_Feedback_Success;
}
#endif



#ifdef  MOTO_Y_VERSION_V4
/* 4.0版本结构 */
UINT8 MT_Y_MoveToPosRel(CALL_STYLE_E eCall)
{
    struct tMvMotorPara  tMvoingPara;

	if(eCall == e_NormalCheck_Call)
	{
		//moto_work_stat(1, MOTO_WORK_STAT_RUN);  /* 动作开始执行 */
		moto_work_stat_2(1, MOTO_WORK_STAT_RUN, e_BUILD_PRESS_SUCCESS);
	}
    // record the motor's para
    tMvoingPara.nFreqMin = g_atMotorPara[Motor_Y].nFreqMin;
    tMvoingPara.nFreqMax = g_atMotorPara[Motor_Y].nFreqMax;
    tMvoingPara.nFreqInc = g_atMotorPara[Motor_Y].nFreqInc;
    tMvoingPara.nFreqSam = g_atMotorPara[Motor_Y].nFreqSam;

    // not detect the single of home at the begining, moving long diatance
//	if(1 == HW_LEVEL_GetOC(OC_HOME_CHANNEL)) 
//	{
		if (1 == HW_LEVEL_GetOC(OC_SAMPLE_RELEA_CHANNEL))
		{
			// long distance
			MV_InitPara(Motor_Y, 4000, 8000, 100, 10);
			// OC is on the right, right step
			MV_Move(Motor_Y, 35000, e_Dir_Neg); // comes far the OC

			while (1 == HW_LEVEL_GetOC(OC_SAMPLE_RELEA_CHANNEL)) // OC detection is not enable
			{
				if (e_True == MV_IsFinished(Motor_Y))
				{
					MV_Stop(Motor_Y);
					if(eCall == e_NormalCheck_Call)
					{
						collect_return_hdl(COLLECT_RET_FAIL_SAMPLE);	
						//moto_work_stat(1, MOTO_WORK_STAT_FAIL);  /* 动作执行失败 */
						moto_work_stat_2(1, MOTO_WORK_STAT_FAIL, e_BUILD_PRESS_SUCCESS);
					}
					return e_Feedback_Error;
				}
			}

			/* 增加临时运行时序 */
			MV_Move(Motor_Y, 600, e_Dir_Neg);  /* 继续运行 */
			while (e_False == MV_IsFinished(Motor_Y));
			/* 增加临时运行时序 */

			IT_SYS_DlyMs(3);
			MV_Stop(Motor_Y);
		}
		else
		{
			/* 增加临时运行时序 */
			MV_InitPara(Motor_Y, 4000, 8000, 100, 10);
			MV_Move(Motor_Y, 20000, e_Dir_Pos);

			while (0 == HW_LEVEL_GetOC(OC_SAMPLE_RELEA_CHANNEL)) // OC detection is not enable
			{
				if (e_True == MV_IsFinished(Motor_Y))
				{
					MV_Stop(Motor_Y);
					if(eCall == e_NormalCheck_Call)
					{
						collect_return_hdl(COLLECT_RET_FAIL_SAMPLE);
						//moto_work_stat(1, MOTO_WORK_STAT_FAIL);  /* 动作执行失败 */
						moto_work_stat_2(1, MOTO_WORK_STAT_FAIL, e_BUILD_PRESS_SUCCESS);
					}
					return e_Feedback_Error;
				}
			}

			MV_Move(Motor_Y, 600, e_Dir_Neg);  /* 继续运行 */
			while (e_False == MV_IsFinished(Motor_Y));

			IT_SYS_DlyMs(3);
			MV_Stop(Motor_Y);
			/* 增加临时运行时序 */
		}
//	}

    //
    g_tAxisPosStatus.nAxisY = 0;
    g_tAxisPosStatus.eAxisY = E_AXIS_Y_POS_HOME;
    //---------------------------------------------------
    // set to the default moving parameters
    MV_InitPara(Motor_Y,
                tMvoingPara.nFreqMin,
                tMvoingPara.nFreqMax,
                tMvoingPara.nFreqInc,
                tMvoingPara.nFreqSam);
    //
	if(eCall == e_NormalCheck_Call)
	{
		//moto_work_stat(1, MOTO_WORK_STAT_OK);  /* 动作执行成功 */
		moto_work_stat_2(1, MOTO_WORK_STAT_OK, e_BUILD_PRESS_SUCCESS);
	}
    return e_Feedback_Success;
}

#else

UINT8 MT_Y_MoveToPosRel(void)
{
    // MV_InitPara(Motor_Y, 4000, 8000, 100, 10);
    //

    //moto_work_stat(1, MOTO_WORK_STAT_RUN);
	moto_work_stat_2(1, MOTO_WORK_STAT_RUN, e_BUILD_PRESS_SUCCESS);
    if (E_AXIS_Y_POS_UNSURE == g_tAxisPosStatus.eAxisY)
    {
        MT_Y_Home();
    }
    //
    if (E_AXIS_Y_POS_HOME == g_tAxisPosStatus.eAxisY)
    {
        MV_Move(Motor_Y, 2000, e_Dir_Neg);
        //
        g_tAxisPosStatus.eAxisY = E_AXIS_Y_POS_CTRL;
    }
    //

    //moto_work_stat(1, MOTO_WORK_STAT_OK);
	moto_work_stat_2(1, MOTO_WORK_STAT_OK, e_BUILD_PRESS_SUCCESS);
    return e_Feedback_Success;
}
#endif

//------------------------------
// FPGA INIT

// RESET
//

void  HW_FPGA_RST_H(void)
{
	  IO_ UINT32 IRAM_  nAddr     = 0;
      IO_ UINT16 IRAM_  anBuffer[2];
	
	  nAddr = (UINT32)FPGA_WR_MOTOR_PARA_OFFSET;
	  // value
	  anBuffer[0] = 0xFFFF;
	  // write to the fpga
	  FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
}

void  HW_FPGA_RST_L(void)
{
	  IO_ UINT32 IRAM_  nAddr     = 0;
      IO_ UINT16 IRAM_  anBuffer[2];
	
	  nAddr = (UINT32)FPGA_WR_MOTOR_PARA_OFFSET;
	  // value
	  anBuffer[0] = 0xFFFE;
	  // write to the fpga
	  FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
}
//------------------------------
// I/Os control

// outputs
//
UINT8  HW_Valve_On(UINT8 chIndex)
{
    IO_ UINT8  XRAM_ chOffset = 0;
    IO_ UINT32 IRAM_  nAddr     = 0;
    IO_ UINT16 IRAM_  anBuffer[2];

    // attention: 0-on, 1-off.

    // fpga
    /* if( (chIndex >= 0) && (chIndex <= 5) )    // 0 ~ 5 */
    if (chIndex <= 5)   /* 0 ~ 5 */
    {
        chOffset = chIndex - 0;
        //m_nIoValves |= ((UINT16)1 << chOffset);
        m_nIoValves &= ~((UINT16)1 << chOffset);
        // address
        nAddr = (UINT32)FPGA_WR_VALVE_01_06;
        // value
        anBuffer[0] = m_nIoValves;
        // write to the fpga
        FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
        //
        printf("valve %d on\r\n", chIndex);
    }
    // main mcu
    else if ((chIndex >= 6) && (chIndex <= 20))
    {
        // keep
    }
    // error
    else
    {
        SYS_ErrorMark((UINT8)ERR_COMMAND_NO_VALID, chIndex);
        return e_Feedback_Error;
    }

    return e_Feedback_Success;
}

//
UINT8  HW_Valve_Off(UINT8 chIndex)
{
    IO_ UINT8  XRAM_ chOffset = 0;
    IO_ UINT32 IRAM_  nAddr 	= 0;
    IO_ UINT16 IRAM_  anBuffer[2];

    // attention: 0-on, 1-off.

    // fpga
    /* if( (chIndex >= 0) && (chIndex <= 5) )    // 0 ~ 5 */
    if (chIndex <= 5)
    {
        /* if( (chIndex >= 0) && (chIndex <= 15) ) */
        if (chIndex <= 15)
        {
            chOffset = chIndex - 0;
            m_nIoValves |= ((UINT16)1 << chOffset);
            //m_nIoValves &= ~((UINT16)1 << chOffset);
            // address
            nAddr = (UINT32)FPGA_WR_VALVE_01_06;
            // value
            anBuffer[0] = m_nIoValves;
        }
        // write to the fpga
        FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
        //
        printf("valve %d off\r\n", chIndex);
    }
    // main mcu
    else if ((chIndex >= 6) && (chIndex <= 20))
    {
        // keep
    }
    // error
    else
    {
        SYS_ErrorMark((UINT8)ERR_COMMAND_NO_VALID, chIndex);
        return e_Feedback_Error;
    }

    return e_Feedback_Success;
}



//------------------------------
// DC motor control
/* 有效频率范围15K - 25KHz */
UINT8  HW_PUMP_Pulse(UINT32 nFreq, enum eDirection eDir)
{
    IO_ UINT32 IRAM_  nAddr 	= 0;
    IO_ UINT16 IRAM_  anBuffer[2];
    //
    IO_ UINT32 XRAM_  nFqCnt    = 0;

    //----- 1. direction -----
    // address
    nAddr = (UINT32)FPGA_WR_PUMP_DIR;
    // value
    if (e_Dir_Neg == eDir)
    {
        anBuffer[0] = 0x00000000;
    }
    else
    {
        anBuffer[0] = 0x00000001;
    }
    // write the fpga
    FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
    //----- 2. frequence -----
    // address
    nAddr = (UINT32)FPGA_WR_PUMP_FQ_CNT;
    // value
    // attentio: the fpga's sysclk = 25000000Hz
#if 0  // 2015_04_08-11shi-changed by LHT	
    nFqCnt = 12500000 / (nFreq + 1);      // nFreq != 0, half-freq-count, 12.5MHz/nFreq
#else
    if (nFreq >= 25000)
    {
        nFqCnt = 25000;
    }
    else
    {
        nFqCnt = nFreq;
    }
#endif
    anBuffer[0] = (UINT16)(nFqCnt & 0xFFFF);
    anBuffer[1] = (UINT16)((nFqCnt >> 16) & 0xFFFF);
    // write the fpga
    FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 2); // 2 half-word
    //----- 3. run or stop -----
    // address
    nAddr = (UINT32)FPGA_WR_PUMP_RUN;
    // value
    if (0 == nFreq)  // stop
    {
        anBuffer[0] = 0x00000000;
        //
        printf("pump off\r\n");
    }
    else             // run
    {
        anBuffer[0] = 0x00000001;
        //
        printf("pump on at %0.5d ticks per ms (total 25000 ticks per ms)\r\n", (int)nFqCnt);
    }
    // write the fpga
    FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word

    return e_Feedback_Success;
}

//
UINT32 HW_PUMP_GetFeedbackPulse(void)
{
    IO_ UINT32 IRAM_  nAddr 	= 0;
    IO_ UINT16 IRAM_  anBuffer[2];
    //
    IO_ UINT32 XRAM_  nValue    = 0;

    //----- feedback pulse -----
    // address
    nAddr = (UINT32)FPGA_RD_FREQ_PUMP;
    // read from the fpga
    FPGA_ReadBuffer((UINT16 *)anBuffer, nAddr, 2); // 2 half-word
    nValue = (UINT32)(((UINT32)anBuffer[1]) << 16) + anBuffer[0];

    return nValue;
}

//------------------------------
// the digtal adjustable resistor
UINT8  HW_ADJ_SetResistor(UINT8 chIndex, UINT8 chValue)
{
    IO_ UINT32 IRAM_  nAddr 	= 0;
    IO_ UINT16 IRAM_  anBuffer[2];
    //
    // IO_ UINT32 XRAM_  nValue    = 0;

    // address
    switch (chIndex)
    {
        case 0:
        {
            nAddr = (UINT32)FPGA_WR_ADJ_RES_01;
            break;
        }
        case 1:
        {
            nAddr = (UINT32)FPGA_WR_ADJ_RES_02;
            break;
        }
        case 2:
        {
            nAddr = (UINT32)FPGA_WR_ADJ_RES_03;
            break;
        }
        case 3:
        {
            nAddr = (UINT32)FPGA_WR_ADJ_RES_04;
            break;
        }
        default:
        {
            // do nothing
            break;
        }
    }
    //
    anBuffer[0] = chValue & 0xFFFF;
    // write the fpga
    FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
    //
    return e_Feedback_Success;
}

//------------------------------
// the SPI ADC control
UINT8  HW_ADC_SpiCtrl(enum eFlag bAble)
{
    IO_ UINT32 IRAM_  nAddr 	= 0;
    IO_ UINT16 IRAM_  anBuffer[2];
    // IO_ UINT32 XRAM_  nValue    = 0;

    // address
    nAddr = (UINT32)FPGA_WR_SPI_ADC_CTRL;
    // value
    if (e_False == bAble)
    {
        anBuffer[0] = (UINT16)(FPGA_WR_V_SPI_ADC_OFF & 0xFFFF);
    }
    else
    {
        anBuffer[0] = (UINT16)(FPGA_WR_V_SPI_ADC_ON & 0xFFFF);
    }
    // write the fpga
    FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
    //
    return e_Feedback_Success;
}

//
UINT16 HW_ADC_SpiGetADC(UINT8 chIndex)
{
    IO_ UINT32 IRAM_  nAddr 	= 0;
    IO_ UINT16 IRAM_  anBuffer[2];

    switch (chIndex)
    {
        case 0:
        {
            //
            nAddr = (UINT32)FPGA_RD_SPI_ADC_01;
            //
            FPGA_ReadBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
            //
            break;
        }
        case 1:
        {
            //
            nAddr = (UINT32)FPGA_RD_SPI_ADC_02;
            //
            FPGA_ReadBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
            //
            break;
        }
        case 2:
        {
            //
            nAddr = (UINT32)FPGA_RD_SPI_ADC_03;
            //
            FPGA_ReadBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
            //
            break;
        }
        case 3:
        {
            //
            nAddr = (UINT32)FPGA_RD_SPI_ADC_04;
            //
            FPGA_ReadBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
            //
            break;
        }
        default:
        {
            //
            anBuffer[0] = 0;
            //
            break;
        }
    }
    //
    return (anBuffer[0] & 0x0FFF);  // the lower 12-bit
}

// * 10000
INT32 HW_ADC_SpiGetPress(void)
{
    //
    UINT16 nAd    = 0;
    INT32 nValue = 0;
    double fValue = 0;

	#if USE_STM32F407_ONLY
		nAd = Get_Press_ADC();
	#else
		nAd = HW_ADC_SpiGetADC(INDEX_PRESS);  // 0: HGB, 1: press
	#endif
    //

    fValue = nAd * ((double)s_nK);
    if (fValue <= (double)s_nB)
    {
		//return g_Record_Param.nAddPress;
        return 0;
    }

    fValue -= (double)s_nB;
	//nValue = (UINT32)fValue
    nValue = (INT32)fValue + g_Record_Param.nAddPress; 
	printf("Get press: addP=%d,oriP=%d,P=%d",\
				(int)g_Record_Param.nAddPress, (int)fValue, (int)nValue);
    return nValue;
}

//
UINT32 HW_ADC_PressPara(UINT32 nK, UINT32 nB)
{
    s_nK = nK;
    s_nB = nB;
    //
    return e_Feedback_Success;
}

//------------------------------
// get the level of the OC and the electrode
/* 预留的悬空位返回状态1表示光耦未被遮挡 */
UINT8  HW_LEVEL_GetOC(UINT8 chIndex)
{
    IO_ UINT32 IRAM_  nAddr 	= 0;
    IO_ UINT16 IRAM_  anBuffer[2];
    IO_ UINT8  IRAM_  chValue   = 1;  /* 预留的悬空位返回状态1表示光耦未被遮挡 */

    //
    nAddr = (UINT32)FPGA_RD_OC_01_03;
    //
    FPGA_ReadBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
    //
    switch (chIndex)
    {
        case 0:
        {
            if (0 != (anBuffer[0] & 0x0001))
                chValue = 0x01;
            else
                chValue = 0x00;
            break;
        }
        case 1:
        {
            if (0 != (anBuffer[0] & 0x0002))
                chValue = 0x01;
            else
                chValue = 0x00;
            break;
        }
        case 2:
        {
            if (0 != (anBuffer[0] & 0x0004))
                chValue = 0x01;
            else
                chValue = 0x00;
            break;
        }
        case 3:
        {
            if (0 != (anBuffer[0] & 0x0008))
                chValue = 0x01;
            else
                chValue = 0x00;
            break;
        }
        default:
        {
            break;
        }
    }

    return chValue;
}

UINT8  HW_LEVEL_GetElectrode(UINT8 chIndex)
{
    IO_ UINT32 IRAM_  nAddr 	= 0;
    IO_ UINT16 IRAM_  anBuffer[2];
    IO_ UINT8  IRAM_  chValue;

    //
    nAddr = (UINT32)FPGA_RD_ELECTRODE_01_04;
    //
    FPGA_ReadBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
    //
    switch (chIndex)
    {
        case 0:
        {
            if (0 != (anBuffer[0] & 0x0001))
                chValue = 0x01;
            else
                chValue = 0x00;
            break;
        }
        case 1:
        {
            if (0 != (anBuffer[0] & 0x0002))
                chValue = 0x01;
            else
                chValue = 0x00;
            break;
        }
        case 2:
        {
            if (0 != (anBuffer[0] & 0x0004))
                chValue = 0x01;
            else
                chValue = 0x00;
            break;
        }
        case 3:
        {
            if (0 != (anBuffer[0] & 0x0008))
                chValue = 0x01;
            else
                chValue = 0x00;
            break;
        }
        default:
        {
            break;
        }
    }

    return chValue;
}

/* 滤波方式获取电极状态 */
#define  ELECTRODE_GET_FILTER_NUM  5
UINT8 hw_filter_get_electrode(UINT8 chIndex)
{
    UINT8 n, cnt;

    cnt = 0;
    for (n = 0; n < ELECTRODE_GET_FILTER_NUM; n++)
    {
        if (1 == HW_LEVEL_GetElectrode(chIndex))
        {
            cnt += 1;
        }
    }

    if (cnt > (ELECTRODE_GET_FILTER_NUM / 2))  /* 以超过半数的值为准 */
    {
        return  1;
    }

#ifdef   DEBUG_TEST  /* 临时调试用，设置电极永远无法获取溢出信号 */
    return  1;
#else
    return  0;
#endif
}

//------------------------------
// the ADC of the slave CPU
UINT16 HW_ADC_SlaveGetADC(void)
{
    _STA_ UINT16 nValue = 0;
    //
    if (E_PL_BUF_ENABLE == SPG_IsRecvCommandValid())
    {
        nValue = (((UINT16)g_SPG_achCmdRvBuf[1]) << 8) + g_SPG_achCmdRvBuf[2];
        //
        SPG_ResetRecvComand();
    }
    else
    {
        nValue = nValue;
    }
    //
    return nValue;
}

//------------------------------
// to control the direction
UINT8  HW_DIR_Ctrl(void)
{
    IO_ UINT32 IRAM_  nAddr 	= 0;
    IO_ UINT16 IRAM_  anBuffer[2];
    IO_ UINT8  IRAM_  chValue;

    //
    nAddr = (UINT32)FPGA_WR_DIRECTION_BOARD;
    //
    anBuffer[0] = (UINT16)FPGA_WR_V_BOARD_1_OUT + FPGA_WR_V_BOARD_2_IN;
    // write the fpga
    FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
    //
    return e_Feedback_Success;
}

//------------------------------
// to control the switcher
UINT8  HW_SW_AdcWBC(enum eFlag bSw)
{
    IO_ UINT32 IRAM_  nAddr 	= 0;
    IO_ UINT16 IRAM_  anBuffer[2];
    IO_ UINT8  IRAM_  chValue;
    //
    nAddr = (UINT32)FPGA_WR_SW_WBC;
    //
    if (e_False == bSw)
    {
        anBuffer[0] = (UINT16)FPGA_WR_V_SW_WBC_OFF;
    }
    else
    {
        anBuffer[0] = (UINT16)FPGA_WR_V_SW_WBC_ON;
    }
    // write the fpga
    FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
    //
    return e_Feedback_Success;
}

UINT8  HW_SW_AdcRBC(enum eFlag bSw)
{
    IO_ UINT32 IRAM_  nAddr 	= 0;
    IO_ UINT16 IRAM_  anBuffer[2];
    IO_ UINT8  IRAM_  chValue;
    //
    nAddr = (UINT32)FPGA_WR_SW_RBC;
    //
    if (e_False == bSw)
    {
        anBuffer[0] = (UINT16)FPGA_WR_V_SW_WBC_OFF;
    }
    else
    {
        anBuffer[0] = (UINT16)FPGA_WR_V_SW_WBC_ON;
    }
    // write the fpga
    FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
    //
    return e_Feedback_Success;
}

//------------------------------
// enable the WBC
UINT8  HW_EN_WBC(enum eFlag bOn)
{
    IO_ UINT32 IRAM_  nAddr 	= 0;
    IO_ UINT16 IRAM_  anBuffer[2];
    IO_ UINT8  IRAM_  chValue;
    //
    nAddr = (UINT32)FPGA_WR_ENABLE_WBC;
    //
    if (e_False == bOn)
    {
        anBuffer[0] = (UINT16)FPGA_WR_V_ENABLE_WBC_OFF;
    }
    else
    {
        anBuffer[0] = (UINT16)FPGA_WR_V_ENABLE_WBC_ON;
    }
    // write the fpga
    FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
    //
    return e_Feedback_Success;
}

//
void  HW_Start_WBC(void)
{
#if 0
    HW_SW_AdcWBC(e_True);          /* switch on : WBC and RBC */
    HW_SW_AdcRBC(e_False);		   // RBC is back channel for WBC
    HW_EN_WBC(e_True);             /* get the ADC data */
#else	
	memset((void*)&ADC_Status, 0, sizeof(ADC_Status_InitTypeDef));
	ADC1_Init();
	ADC_SoftwareStartConv(ADC1);
#endif
}

//
void  HW_End_WBC(void)
{
#if 0
    HW_SW_AdcWBC(e_False);
    HW_SW_AdcRBC(e_False);	// RBC is back channel for WBC
    // close the ADC channel
    HW_EN_WBC(e_False);
#else
	ADC_Cmd(ADC1, DISABLE);
	ADC_DMACmd(ADC1, DISABLE);
	DMA_Cmd(DMA2_Stream0, DISABLE);
#endif
}

//
void Send_Last_FIFO_Data(void)
{
	UINT8 i;
	for(i = 0; i < 9; i++) // FPGA FIFO is 4K, the last data should less than 4K
	{
		HW_LWIP_Working(IT_LIST_GetTicks(), IT_ADC_GetTicks(), EN_SEND_FPGA_DATA);
	}
}

void Clear_FPGA_FIFO_Data(void)
{
	UINT8 i;
	for(i = 0; i < 8; i++)
	{
		HW_DATA_ClearData(((UINT16 *)(s_anBufNet)), (UINT16 *)&s_nDataLen, (UINT16 *)&s_nStatus);
	}
	printf("Clear_FPGA_FIFO_Data Finished\r\n");
}

//
UINT8 HW_DATA_ClearData(UINT16 * pnData, UINT16 * pnLen, UINT16 * pnStatus)
{
	IO_ UINT32 IRAM_  nAddr 	= 0;
    IO_ UINT16 IRAM_  anBuffer[2];
    //-----------------------------------
    // 1. lock the data-buffer
    nAddr       = (UINT32)FPGA_WR_FLAG_FSMC_READING;
    anBuffer[0] = 0x0001;
    FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
	
//	//-----------------------------------
//	// 2. read the status of the "flag ready"
//	nAddr = (UINT32)FPGA_RD_FLAG_FSMC_READY;
//	FPGA_ReadBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
//	//
//	*pnStatus = anBuffer[0] >> 4;
////	printf("%X,", anBuffer[0]);
//	// no ready
//	if (0 == (anBuffer[0] & 0x0001))
//	{
//		//-----------------------------------
//		// 4. relase the data-buffer
//		nAddr       = (UINT32)FPGA_WR_FLAG_FSMC_READING;
//		anBuffer[0] = 0x0000;
//		FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
//		//
//		return e_Feedback_Error;
//	}
//    //-----------------------------------
    // 3.
    nAddr = (UINT32)PFGA_RD_DATA_BASE;
    *pnLen = 256;
    FPGA_DATA_ReadBuffer(pnData, nAddr, 256);
    //-----------------------------------
    // 4. relase the data-buffer
    nAddr       = (UINT32)FPGA_WR_FLAG_FSMC_READING;
    anBuffer[0] = 0x0000;
    FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
    //
    return e_Feedback_Success;
}

//------------------------------
//
UINT8  HW_DATA_GetData(UINT16 * pnData, UINT16 * pnLen, UINT16 * pnStatus)
{
    IO_ UINT32 IRAM_  nAddr 	= 0;
    IO_ UINT16 IRAM_  anBuffer[2];
    //-----------------------------------
    // 1. lock the data-buffer
    nAddr       = (UINT32)FPGA_WR_FLAG_FSMC_READING;
    anBuffer[0] = 0x0001;
    FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
	
	//-----------------------------------
	// 2. read the status of the "flag ready"
	nAddr = (UINT32)FPGA_RD_FLAG_FSMC_READY;
	FPGA_ReadBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
	//
	*pnStatus = anBuffer[0] >> 4;
//	printf("%X,", anBuffer[0]);
	// no ready
	if (0 == (anBuffer[0] & 0x0001))
	{
		//-----------------------------------
		// 4. relase the data-buffer
		nAddr       = (UINT32)FPGA_WR_FLAG_FSMC_READING;
		anBuffer[0] = 0x0000;
		FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
		//
		return e_Feedback_Error;
	}
    //-----------------------------------
    // 3.
    nAddr = (UINT32)PFGA_RD_DATA_BASE;
    *pnLen = 256;
    FPGA_DATA_ReadBuffer(pnData, nAddr, 256);
    //-----------------------------------
    // 4. relase the data-buffer
    nAddr       = (UINT32)FPGA_WR_FLAG_FSMC_READING;
    anBuffer[0] = 0x0000;
    FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
    //
    return e_Feedback_Success;
}



UINT8 ADC_Send(UINT32 nCmd, UINT32 nId, UINT16 * pData)
{
	UINT16 i, nData;

	LwIP_Periodic_Handle(IT_SYS_GetTicks());
	s_anBufNet[0] = 0x5344;
    s_anBufNet[1] = 0x4457;
#if 1 //cx2000
	s_anBufNet[2] = (((nId>>24)&0x00FF)|((nId>>8)&0xFF00));
	s_anBufNet[3] = (((nId>>8)&0x00FF) |((nId<<8)&0xFF00));

	//memmove(&s_anBufNet[4], pData, 256);
	for(i = 0; i < 256; i++)
	{
//		nData = *(pData + i);
//		s_anBufNet[4 + i] = (nData>>8)|(nData<<8);
		s_anBufNet[4 + i] = *(pData + i);
		
//		s_anBufNet[6 + i] = *(pData + i);;
	}
	udp_echoserver_senddata(((UINT8 *)(s_anBufNet + 0)), ((256 + 4) * 2));
	
#else //cx3000
	
	s_anBufNet[2] = (((nCmd>>24)&0x00FF)|((nCmd>>8)&0xFF00));
	s_anBufNet[3] = (((nCmd>>8)&0x00FF) |((nCmd<<8)&0xFF00));
	
	s_anBufNet[4] = (((nId>>24)&0x00FF)|((nId>>8)&0xFF00));
	s_anBufNet[5] = (((nId>>8)&0x00FF) |((nId<<8)&0xFF00));

	//memmove(&s_anBufNet[4], pData, 256);
	for(i = 0; i < 256; i++)
	{
		nData = *(pData + i);
		s_anBufNet[6 + i] = ((nData&0XFF00)>>8)|((nData&0x00FF)<<8);
		
//		s_anBufNet[6 + i] = *(pData + i);;
	}
	udp_echoserver_senddata(((UINT8 *)(s_anBufNet + 0)), ((256 + 6) * 2));
#endif
	return 0;
}



//
UINT8  Send_Packet_Test(void)
{
    UINT8 chReturn = 0;
	
    s_anBufNet[0] = 0x5344;
    s_anBufNet[1] = 0x4457;
	s_anBufNet[2] = 0x00;
	s_anBufNet[3] = 0x00;

	g_Udp_Count++;
	s_anBufNet[2] = (UINT16)(((g_Udp_Count&0xFF000000) >> 24) | ((g_Udp_Count&0x00FF0000) >> 8));
	s_anBufNet[3] = (UINT16)(((g_Udp_Count&0x000000FF) << 8) | ((g_Udp_Count&0x0000FF00)>>8));
	
	chReturn = udp_echoserver_senddata(((UINT8 *)(s_anBufNet + 0)), ((256 + 4) * 2));
	if (e_Feedback_Fail == chReturn)
	{
		g_Send_Fail++;
		printf("send fail: f=%d,s=%d\r\n", (int)g_Send_Fail, (int)g_Udp_Count);
		IT_SYS_DlyMs(1);
	}
	return chReturn;

}


_EXT_ UINT8  HW_LWIP_Working_Recv_Handle(UINT32 nTickList, UINT32 nTickAdc)
{
    UINT8 chReturn = 0;
    //=====================================================
    // 1. lwip handing
    // 1) period task
    // ---check if any packet received
    if (ETH_CheckFrameReceived())
    {
        // for debug, the "arp frame"
        // printf("-");
        // process received ethernet packet
        LwIP_Pkt_Handle();
        // udp_echoserver_senddata("abcd", 4);
			  //--------------------------------------------
        // 2) message handling
        PL_NET_CheckingFrame((UINT8 *)g_NET_achCmdRvBuf);
        if (E_PL_BUF_ENABLE == PL_NET_IsRecvCommandValid())
        {
            chReturn = MSG_Handling_MsgHandle((UINT8 *)g_NET_achCmdRvBuf, (UINT8 *)g_achFbkSdBuf);
            //
            PL_NET_ResetRecvComand();
        }
    }
    // ---handle periodic timers for LwIP
    LwIP_Periodic_Handle(IT_SYS_GetTicks());
    return chReturn;
}

//------------------------------
//
UINT8  HW_LWIP_Working(UINT32 nTickList, UINT32 nTickAdc,  EN_FPGA_DATA_FLAG eFlag)
{
	//memset((void*)&ADC_Status, 0, sizeof(ADC_Status_InitTypeDef));
	//ADC1_Init();
	//ADC_SoftwareStartConv(ADC1);
#if 1
	if(ADC_Status.nSFlag == 1)
	{
		//ADC_Send(CMD_DATA_NET_TEST, ADC_Status.nID, g_ADC_Buffer);
		ADC_Send(CMD_DATA_TEST_WBC, ADC_Status.nID, g_ADC_Buffer);
		ADC_Status.nSFlag = 0xFF;
		//printf()
		memset(g_ADC_Buffer, 0, ADC_BUFFER_LEN_HALF);
		ADC_Status.nSendID++;
		g_Udp_Count++;
		//g_Udp_Count = ADC_Status.nSendID++;
	}else if(ADC_Status.nSFlag == 2){
		ADC_Send(CMD_DATA_TEST_WBC, ADC_Status.nID, &g_ADC_Buffer[ADC_BUFFER_LEN_HALF]);
		ADC_Status.nSFlag = 0xFF;
		memset(&g_ADC_Buffer[ADC_BUFFER_LEN_HALF], 0, ADC_BUFFER_LEN_HALF);
		ADC_Status.nSendID++;
		g_Udp_Count++;// ADC_Status.nSendID++;
	}	
	
#else
	
    IO_ UINT8 chReturn;
	IO_ UINT16 nTemp;
    //=====================================================
    // 1. lwip handing
    // 1) period task
    // ---check if any packet received
//    if (ETH_CheckFrameReceived())
//    {
//        // for debug, the "arp frame"
//        // printf("-");
//        // process received ethernet packet
//        LwIP_Pkt_Handle();
//        // udp_echoserver_senddata("abcd", 4);
//			  //--------------------------------------------
//        // 2) message handling
//        PL_NET_CheckingFrame((UINT8 *)g_NET_achCmdRvBuf);
//        if (E_PL_BUF_ENABLE == PL_NET_IsRecvCommandValid())
//        {
//            MSG_Handling((UINT8 *)g_NET_achCmdRvBuf, (UINT8 *)g_achFbkSdBuf);
//            //
//            PL_NET_ResetRecvComand();
//        }
//    }
    // ---handle periodic timers for LwIP
    LwIP_Periodic_Handle(IT_SYS_GetTicks());

    //=====================================================
#if 1
    // 2. data feedback
    s_anBufNet[0] = 0x5344;
    s_anBufNet[1] = 0x4457;
	s_anBufNet[2] = 0x00;
	s_anBufNet[3] = 0x00;
	
    chReturn = HW_DATA_GetData(((UINT16 *)(s_anBufNet + 4)), (UINT16 *)&s_nDataLen, (UINT16 *)&s_nStatus);
    // s_anBufNet[3] = s_nStatus;
  
    if (e_Feedback_Success == chReturn)
    {
		g_Frame_Count++;
		if(eFlag == EN_SEND_FPGA_DATA){
			
			g_Udp_Count++;
//			if(g_Udp_Count == 1) printf("\r\n ticks=%d, udp=%d\r\n", (int)nTickList, (int)g_Udp_Count);
//			if(g_Udp_Count == 2) printf("\r\n ticks=%d, udp=%d\r\n", (int)nTickAdc, (int)g_Udp_Count);
			s_anBufNet[2] = (UINT16)(((g_Udp_Count&0xFF000000) >> 24) | ((g_Udp_Count&0x00FF0000) >> 8));
			s_anBufNet[3] = (UINT16)(((g_Udp_Count&0x000000FF) << 8) | ((g_Udp_Count&0x0000FF00)>>8));
			chReturn = udp_echoserver_senddata(((UINT8 *)(s_anBufNet + 0)), ((s_nDataLen + 4) * 2));
			if (e_Feedback_Fail == chReturn)
			{
				g_Send_Fail++;
				IT_SYS_DlyMs(1);
			}
		}
//		else if(eFlag == EN_DROP_FPGA_DATA){// else do not send to app
			
//		}
        //----------------------------------------------------------
        // debug..., printf the get data via serial , 20190315
//      PL_COM_SendNChar(((UINT8 *)(s_anBufNet + 0)), ((s_nDataLen + 4) * 2));
    }else{
		g_Frame_Count++;
	}
		
#endif

#if 0
    //-----------------------------------------------------
    // for testing:
    // take attention: the array "s_achBufNet" must be put
    //				   out of the function "main", or it will
    //				   case error.
    chReturn = udp_echoserver_senddata((UINT8 *)g_achFbkSdBuf, 1200);
    if (e_Feedback_Fail == chReturn)
    {
        IT_SYS_DlyMs(1);
    }
#endif

    //return e_Feedback_Success;
	
#endif
	return e_Feedback_Success;
}


UINT8  HW_LWIP_MainLine(void)
{
//    UINT8 chReturn;
    //=====================================================
    // 1. lwip handing
    // 1) period task
    // ---check if any packet received
    if (ETH_CheckFrameReceived())
    {
        // for debug, the "arp frame"
        // printf("-");
        // process received ethernet packet
        LwIP_Pkt_Handle();
        // udp_echoserver_senddata("abcd", 4);
        //--------------------------------------------
        // 2) message handling
        PL_NET_CheckingFrame((UINT8 *)g_NET_achCmdRvBuf);
        if (E_PL_BUF_ENABLE == PL_NET_IsRecvCommandValid())
        {
            MSG_Handling((UINT8 *)g_NET_achCmdRvBuf, (UINT8 *)g_achFbkSdBuf);
            //
            PL_NET_ResetRecvComand();
        }
    }
    // ---handle periodic timers for LwIP
    LwIP_Periodic_Handle(IT_SYS_GetTicks());

#if 0
    //=====================================================
    // 2. data feedback
    s_anBufNet[0] = 0x5344;
    s_anBufNet[1] = 0x4457;
    s_anBufNet[2] = 0x0001;
    s_anBufNet[3] = 0x0000;
   
	chReturn = HW_DATA_GetData((UINT16 *)(s_anBufNet + 4), (UINT16 *)&s_nDataLen, (UINT16 *)&s_nStatus); 
    s_anBufNet[3] = s_nStatus;
    //
    if (e_Feedback_Success  == chReturn)
    {
       chReturn = udp_echoserver_senddata(((UINT8 *)(s_anBufNet + 0)), ((s_nDataLen + 4) * 2));
        if (e_Feedback_Fail == chReturn)
        {
            IT_SYS_DlyMs(1);
        }
//----------------------------------------------------------
//       debug..., send the last get data via serial
//       PL_COM_SendNChar(((UINT8 *)(s_anBufNet + 0)), ((s_nDataLen + 4) * 2));
    }
#endif	


#if 0
    //-----------------------------------------------------
    // for testing:
    // take attention: the array "s_achBufNet" must be put
    //				   out of the function "main", or it will
    //				   case error.
    chReturn = udp_echoserver_senddata(UINT8 *)(g_achFbkSdBuf, 1200);
    if (e_Feedback_Fail == chReturn)
    {
        IT_SYS_DlyMs(1);
    }
#endif
    return e_Feedback_Success;
}


//-----------------------------------------------------------------------------------------
// reset by software
UINT8 MT_RESET_Software(void)
{
    // attention: no return !
    FPGA_ResetHardware();

    NVIC_SystemReset();

    return e_Feedback_Success;
}

//-----------------------------------------------------------------------------------------
//
UINT8  MSG_SampleBoardFeedback(void)
{
    IO_ UINT8  XRAM_ chData;

    //
    while (1)
    {
        if (e_Feedback_Success != SPG_Recv_GetChar((UINT8 *)&chData))
        {
            break;
        }
        else
        {
            PL_COM_SendChar(chData);
        }
    }

    return e_Feedback_Success;
}







#endif









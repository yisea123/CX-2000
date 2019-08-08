// InterfaceScope.h

#ifndef    __INTERFACE_SCOPE_H__
#define    __INTERFACE_SCOPE_H__

#include  "MyType.h"
//#include  "ProcessScope.h"
//-----------------------------------------------------------------------------------------
// �궨��

#define  OC_HOME_CHANNEL          0          /* ���ֵ�λ����ͨ���� */
#define  OC_OUT_CHANNEL           1          /* ���ֵ�λ����ͨ���� */
#define  OC_SAMPLE_RELEA_CHANNEL  2          /* ����оƬ�ͷŹ���ͨ���� */
#define  OC_SAMPLE_HOLD_CHANNEL   3          /* ����оƬ�ͷŹ���ͨ����(��ʱδʵ��) */
#define  INDEX_PRESS              3          /* ��ѹ���ADCͨ�� ,at FPGA is 4*/
#define  INDEX_ADC_48V            1          /* adc, 0=HGB,1=WBC vol value, 2=RBC(wbc backup), 3=press, */ 
//------------------------------
// 
#define AXIS_X_SPEED_HOME    100000
#define AXIS_X_SPEED_TOP     120000
//
#define AXIS_Y_SPEED_HOME    100000
#define AXIS_Y_SPEED_TOP     120000
//

//------------------------------

// ��΢���ľ�ͷ��
#define SCOPE_POS_NUM  3

// �ƹ�Ŀ��غ궨��
//#define LAMP_ON()    {EVAL_DP_PwrOn(e_True);}
//#define LAMP_OFF()   {EVAL_DP_PwrOn(e_False);}

//------------------------------
// ��������У�����
#define CORRECT_TIME_X  0
#define CORRECT_TIME_Y  0
#define CORRECT_TIME_Z  0
#define CORRECT_TIME_M  0

//------------------------------
// definition
// temperature's range, X 100 
//
#define TEMP_VALUE_MIN           500   //  5�� * 100
#define TEMP_VALUE_MAX           8000  // 80�� * 100
//
#define TEMP_ERR_AUTO_TIMES      10    // before shut down 

#define VALVE_PRESSSURE 		 0  // yaolan_20190220
#define VALVE_SAMPLE     		 1

#define MOTO_SELF_CHECK_TIMEOUT  10000


//-----------------------------------------------------------------------------------------
// data type

// heating type
enum eHeatType
{
    e_HT_OFF       = 0,
	e_HT_HEAT_03   = 1,
	e_HT_HEAT_06   = 2,
	e_HT_HEAT_09   = 3,
	e_HT_HEAT_12   = 4,
	e_HT_HEAT_15   = 5,
	e_HT_HEAT_18   = 6,
	e_HT_HEAT_20   = 7,
	e_HT_HEAT_24   = 8,
};

struct tTempKeep
{
    IO_ enum eFlag  eHold;
	IO_ UINT16      nTemp; 
};

typedef enum
{
    MOTO_WORK_STAT_RUN = 0,  /* ����ִ�� */
    MOTO_WORK_STAT_OK,       /* ִ�гɹ� */
    MOTO_WORK_STAT_FAIL,     /* ִ��ʧ�� */
}
MOTO_WORK_STAT_E;

typedef enum
{
	e_BUILD_PRESS_SUCCESS = 0,
	e_BUILD_PRESS_FAIL = 1,
} BUILD_PRESS_E;

typedef enum
{
	e_Moto_Out = 0,
	e_Moto_In  = 1,
}MOTO_WORK_DIR;

typedef enum {
	e_NormalCheck_Call = 0,  // normal test
	e_SelfCheck_Call   = 1, // for self check
	e_PartTest_Call    = 2	// for part tet
}CALL_STYLE_E;


typedef enum{
	e_Msg_Ctrol  = 0,
	e_Msg_Status = 1,
	e_Msg_Query  = 2,
	e_Msg_Data   = 3,
	e_Msg_End    = 4
} EN_MSG_TYPE;

typedef enum{
	// get or clear for HW_DATA_GetData
	EN_GET_FPGA_DATA   = 0,
	EN_CLEAR_FPGA_DATA = 1,
	// send or drop flag for HW_LWIP_Working
	EN_SEND_FPGA_DATA  = 2,
	EN_DROP_FPGA_DATA  = 3
}EN_FPGA_DATA_FLAG;

//-----------------------------------------------------------------------------------------
// ����Ŀ��ƽӿ�����
// init
_EXT_ UINT8 MT_ALL_Init(void);
// axis x
_EXT_ enum eAxisXPos MT_X_get_posi(void);
//_EXT_ UINT8 MT_X_Home(void);
_EXT_ UINT8 MT_X_Home(CALL_STYLE_E eCall);
//_EXT_ UINT8 MT_X_Home_2(CALL_STYLE_E eCall);
_EXT_ UINT8 MT_X_Home_only(void);

UINT8 MT_X_MoveToPosRel(CALL_STYLE_E eCall);
//_EXT_ UINT8 MT_X_MoveToPosRel(void);
_EXT_ UINT8 MT_X_MoveToPosRel_only(void);

enum eAxisYPos MT_Y_get_posi(void);
UINT8 MT_Y_Home(CALL_STYLE_E eCall);
UINT8 MT_Y_MoveToPosRel(CALL_STYLE_E eCall);

//------------------------------
_EXT_ void  HW_FPGA_RST_H(void);

_EXT_ void  HW_FPGA_RST_L(void);
// I/Os control

//
_EXT_ UINT8  HW_Valve_On(UINT8 chIndex);
//
_EXT_ UINT8  HW_Valve_Off(UINT8 chIndex);

//------------------------------
// DC motor control
//
_EXT_ UINT8  HW_PUMP_Pulse(UINT32 nFreq, enum eDirection eDir);
// 
_EXT_ UINT32 HW_PUMP_GetFeedbackPulse(void);

//------------------------------
// the digtal adjustable resistor 
_EXT_ UINT8  HW_ADJ_SetResistor(UINT8 chIndex, UINT8 chValue);


//------------------------------
// the SPI ADC control
_EXT_ UINT8  HW_ADC_SpiCtrl(enum eFlag bAble);
_EXT_ UINT16 HW_ADC_SpiGetADC(UINT8 chIndex);
_EXT_ INT32 HW_ADC_SpiGetPress(void);
_EXT_ UINT32 HW_ADC_PressPara(UINT32 nK, UINT32 nB);

//------------------------------
// get the level of the OC and the electrode
_EXT_ UINT8  HW_LEVEL_GetOC(UINT8 chIndex);
_EXT_ UINT8  HW_LEVEL_GetElectrode(UINT8 chIndex);
_EXT_ UINT8 hw_filter_get_electrode(UINT8 chIndex);

//------------------------------
// the ADC of the slave CPU 
_EXT_ UINT16 HW_ADC_SlaveGetADC(void);

//------------------------------
// to control the direction 
_EXT_ UINT8  HW_DIR_Ctrl(void);

//------------------------------
// to control the switcher
_EXT_ UINT8  HW_SW_AdcWBC(enum eFlag bSw);
_EXT_ UINT8  HW_SW_AdcRBC(enum eFlag bSw);

//------------------------------
// enable the WBC
_EXT_ UINT8  HW_EN_WBC(enum eFlag bOn);
_EXT_ void  HW_Start_WBC(void);
_EXT_ void  HW_End_WBC(void);


void Send_Last_FIFO_Data(void);
//------------------------------
// get the data from the data buffer 
_EXT_ UINT8  HW_DATA_GetData(UINT16* pnData, UINT16* pnLen, UINT16* pnStatus);
_EXT_ UINT8 HW_DATA_ClearData(UINT16 * pnData, UINT16 * pnLen, UINT16 * pnStatus);
void Clear_FPGA_FIFO_Data(void);
//------------------------------
// 
_EXT_ UINT8  HW_LWIP_Working(UINT32 nTickList, UINT32 nTickAdc, EN_FPGA_DATA_FLAG eFlag);
_EXT_ UINT8  HW_LWIP_MainLine(void);
//
_EXT_ UINT8  HW_LWIP_Working_Recv_Handle(UINT32 nTickList, UINT32 nTickAdc);

_EXT_ UINT8  Send_Packet_Test(void);

//-----------------------------------------------------------------------------------------
// reset by software 
_EXT_ UINT8 MT_RESET_Software(void);


//-----------------------------------------------------------------------------------------
//
_EXT_ UINT8  MSG_SampleBoardFeedback(void);

UINT8 ADC_Send(UINT32 nCmd, UINT32 nId, UINT16 * pData); ////////////////////

#endif

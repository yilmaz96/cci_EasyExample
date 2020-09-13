/* ************************************************************************ */
/*!
   \file           App_VTClientLev2.h

   \brief          Header file for VT client demo/workshop functions

   \author         Erwin Hammerl
   \date           Created 15.12.2013

   Copyright (c) WBZ-Engineering GmbH

   \par HISTORY:

*/
/* ************************************************************************ */

#ifndef __APPISO_VTLEV2_H
#define __APPISO_VTLEV2_H

#ifdef __cplusplus
extern "C" {
#endif
/* ************************************************************************ */

#define WS_OBJECT             0u
#define DM_PAGE1           1000u
#define DM_PAGE2           1001u
#define DM_PAGE3           1002u
#define KEY_NEXTPAGE       5100u
#define KEY_RESETCNT       5101u
#define KEY_CNT            5102u
#define KEY_F1             5104u
#define OUTNU_NUM1        12200u
#define OUTNU_NUM2        21001u
#define OUTSTR_L1         22000u
#define OUTSTR_L2         22001u
#define OUTSTR_L3         22002u

void VTC_setNewVT(iso_u8 u8Instance);
void VTC_setPoolManipulation(iso_u8 u8Instance);

void VTC_handleSoftkeys(const ISOVT_MSG_STA_T * pIsoMsgSta);
void VTC_setPage2(void);

//void VTC_process_VT_change_string_value(const ISOVT_MSG_STA_T * pIsoMsgSta);
//void VTC_process_Auxiliary_input_Type2(const ISOVT_MSG_STA_T * pIsoMsgSta);
//void VTC_ReadLCofWS(void);
//void VTC_AuxUnitType1_detected(void);
//void VTC_Sample_cyclic(void);

/* ************************************************************************ */
#ifdef __cplusplus
} /* end of extern "C" */
#endif 
#endif /* __APPISO_VTLEV2_H */
/* ************************************************************************ */

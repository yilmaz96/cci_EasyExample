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


void VTC_setNewVT(iso_u8 u8Instance);
void VTC_setPoolReady(iso_u8 u8Instance);
void VTC_setPoolManipulation(iso_u8 u8Instance);

void VTC_handleSoftkeysAndButtons(const struct ButtonActivation_S *pButtonData);
void VTC_handleAux(const struct AUX_InputSignalData_T *InputSignalData);


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

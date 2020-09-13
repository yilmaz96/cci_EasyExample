/* ************************************************************************ */
/*!
   \file

   \brief          VT client demo application

   \author         Erwin Hammerl
   \date           Created 17.09.2007

   \copyright  Wegscheider Hammerl Ingenieure Partnerschaft

   \par HISTORY:

*/
/* **************************  includes ********************************** */

#include "IsoDef.h"

#ifdef _LAY6_  /* compile only if VT client is enabled */


#include "VIEngine.h"
#include "VIEngine.h"
#include "App_VTClientLev2.h"   // -> Object defines

#include "MyProject1.iop.h"
#include "Settings/settings.h"


// called from AppPoolSettings()
void VTC_setPoolManipulation(iso_u8 u8Instance)
{
   iso_u16  u16DM_Scal  = 10000u;          // Scaling factor * 10000
   iso_u16  u16SKM_Scal = 10000u;

   // ------------------------------------------------------------------------------

   // IsoVtcPoolSetIDRangeMode(u8Instance, 0, 60000, 10000, NoScaling);          // Switch off automatic scaling

   u16DM_Scal = (iso_u16)IsoVtcPoolReadInfo(u8Instance, PoolDataMaskScalFaktor);       // Call only after PoolInit !!
   u16SKM_Scal = (iso_u16)IsoVtcPoolReadInfo(u8Instance, PoolSoftKeyMaskScalFaktor);

   IsoVtcPoolSetIDRangeMode(u8Instance, 5100u, 5300u, u16SKM_Scal, Centering);       // Scale and center Keys
   IsoVtcPoolSetIDRangeMode(u8Instance, 20700u, 20799u, u16SKM_Scal, Scaling);         // Scale Pictures in keys


   // ------------------------------------------------------------------------------


   IsoVtcPoolSetIDRangeMode(u8Instance, 0u,     0u, u16SKM_Scal, Centering);  // Working set object
   IsoVtcPoolSetIDRangeMode(u8Instance, 20000u, 20000u, u16SKM_Scal, Scaling);    // Working set designator
   IsoVtcPoolSetIDRangeMode(u8Instance, 29000u, 29099u, u16SKM_Scal, Centering);  // Auxiliary function
   IsoVtcPoolSetIDRangeMode(u8Instance, 20900u, 20999u, u16SKM_Scal, Scaling);    // Auxiliary bitmaps
   (void)u16DM_Scal;


	if (IsoVtcGetStatusInfo(u8Instance, VT_VERSIONNR) == VT_V2_FE)
	{
		// Transforming Auxiliary function Type 2 into Type 1
		IsoVtcPoolSetIDRangeMode(u8Instance, 29000, 29999, 0, AuxToV2);
	}
}


iso_u32 Tageszaehler = 0;
iso_u32 Gesamtzaehler = 0;
iso_u32 Hugo = 0;

void VTC_handleSoftkeysAndButtons_RELEASED(const struct ButtonActivation_S *pButtonData)
{

	// what button was released
	switch (pButtonData->objectIdOfButtonObject) {


	case SoftKey_PlusPlus:
	case Button_PlusPlus:
		Tageszaehler++;
		Gesamtzaehler++;
		break;


	case SoftKey_Reset_Gesamtzaehler:
	case Button_Reset_Gesamtzaehler:
		Gesamtzaehler = 0;
		break;


	case SoftKey_Reset_Tageszaehler:
	case Button_Reset_Tageszaehler:
		Tageszaehler= 0;
		break;


	default:
		break;
	}
	IsoVtcCmd_NumericValue(pButtonData->u8Instance, NumberVariable_Tageszaehler, Tageszaehler);
	IsoVtcCmd_NumericValue(pButtonData->u8Instance, NumberVariable_Gesamtzaehler, Gesamtzaehler);
	setU32("CF-A", "Tageszaehler", Tageszaehler);
	setU32("CF-A", "Gesamtzaehler", Gesamtzaehler);
}


void VTC_setNewVT(iso_u8 u8Instance)
{
   // ------------------------------------------------------------------------------

   PARAM_UNUSED(u8Instance);


   // ------------------------------------------------------------------------------
}

void VTC_setPoolReady(iso_u8 u8Instance)
{
	Tageszaehler = getU32("CF-A", "Tageszaehler", 0);
	Gesamtzaehler = getU32("CF-A", "Gesamtzaehler", 0);
	IsoVtcCmd_NumericValue(u8Instance, NumberVariable_Tageszaehler, Tageszaehler);
	IsoVtcCmd_NumericValue(u8Instance, NumberVariable_Gesamtzaehler, Gesamtzaehler);
}




void VTC_handleSoftkeysAndButtons(const struct ButtonActivation_S *pButtonData)
{
	switch (pButtonData->keyActivationCode)
	{
	case BUTTON_STATE_RELEASED:
		VTC_handleSoftkeysAndButtons_RELEASED(pButtonData);
		break;
	case BUTTON_STATE_PRESSED:
		//BUTTON_InputSignalCallback_PRESSED(pButtonData);
		break;
	case BUTTON_STATE_HELD:
		//BUTTON_InputSignalCallback_HELD(pButtonData);
		break;
	case BUTTON_STATE_ABORTED:
		//BUTTON_InputSignalCallback_ABORTED(pButtonData);
		break;
	}
}



/* ************************************************************************ */
#endif /* _LAY6_ */
/* ************************************************************************ */

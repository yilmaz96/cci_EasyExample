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

#include "App_VTClientLev2.h"   // -> Object defines


void VTC_setNewVT(iso_u8 u8Instance)
{
   // ------------------------------------------------------------------------------

   PARAM_UNUSED(u8Instance);


   // ------------------------------------------------------------------------------
}

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

   // Objects to be reloaded
   IsoVtcPoolSetIDRangeMode(u8Instance, 1100u,  1100u, 0u, NotLoad);
   IsoVtcPoolSetIDRangeMode(u8Instance, 40000u, 42000u, 0u, NotLoad);
   IsoVtcPoolSetIDRangeMode(u8Instance, 35000u, 35000u, 0u, NotLoad);

   IsoVtcPoolSetIDRangeMode(u8Instance, 0u,     0u, u16SKM_Scal, Centering);  // Working set object
   IsoVtcPoolSetIDRangeMode(u8Instance, 20000u, 20000u, u16SKM_Scal, Scaling);    // Working set designator
   IsoVtcPoolSetIDRangeMode(u8Instance, 29000u, 29099u, u16SKM_Scal, Centering);  // Auxiliary function
   IsoVtcPoolSetIDRangeMode(u8Instance, 20900u, 20999u, u16SKM_Scal, Scaling);    // Auxiliary bitmaps
   (void)u16DM_Scal;
}

void VTC_handleSoftkeys(const ISOVT_MSG_STA_T * pIsoMsgSta)
{
   switch (pIsoMsgSta->wObjectID)
   {
   case KEY_NEXTPAGE:   // soft key F5 (book)
      if (pIsoMsgSta->lValue == 1)
      {
         if (IsoVtcGetStatusInfo(pIsoMsgSta->u8Instance, ID_VISIBLE_DATA_MASK) == DM_PAGE1)
         {
            IsoVtcCmd_ActiveMask(pIsoMsgSta->u8Instance, WS_OBJECT, DM_PAGE2);  // next page with parent ID
         }
         else if (IsoVtcGetStatusInfo(pIsoMsgSta->u8Instance, ID_VISIBLE_DATA_MASK) == DM_PAGE2)
         {
            IsoVtcCmd_ActiveMask(pIsoMsgSta->u8Instance, WS_OBJECT, DM_PAGE3);  // next page with parent ID
         }
         else { /* unused */ }
      }
      break;
   // ------------------------------------------------------------------------------
   case KEY_RESETCNT:    // soft key F2

      break;
   case KEY_CNT:    // soft key F3

      break;
   case KEY_F1:    // soft key F1

      break;
   // ------------------------------------------------------------------------------
   default:
      break;
   }
}


void VTC_setPage2( void )
{

}

/* ************************************************************************ */
#endif /* _LAY6_ */
/* ************************************************************************ */

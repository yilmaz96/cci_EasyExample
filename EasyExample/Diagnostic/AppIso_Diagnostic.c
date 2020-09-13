/* ************************************************************************ */
/*
   This is an example based on CCI's A3 aux device.
*/
/* ************************************************************************ */

#include "IsoBaseApi.h"
#include "IsoMnApi.h"
#include "AppIso_Diagnostic.h"

#define MINIMUM_CF  1

#if(0)
/*  identical information across all CF's within a device */
static const iso_char ecuPartNumber[] = "CCI A3 part number";           /* 11783-12 -- A.1 ECU part number */
static const iso_char ecuSerialNumber[] = "0815";                       /* 11783-12 -- A.2 ECU serial number -- unique number */
static const iso_char ecuLocation[] = "Cabin";                          /* 11783-12 -- A.7 ECU location */
static const iso_char ecuType[] = "CCI A3";                             /* 11783-12 -- A.8 ECU type */
static const iso_char ecuManufacturerName[] = "CCI e.V.";               /* 11783-12 -- A.5 manufactuer name*/
static const iso_char ecuHardwareVersionId[] = "000209#A3 v.0.9";       /* 11783-12 -- A.21 ECU hardware version identification;
                                                                                   209 is assigned through AEF database 
                                                                                   A3 v.0.9 is optional additional information */

static const iso_char swId1[] = "AUX v0.1 20180217";                    /* 11783-12 -- A.4 software identification */
static const iso_char swId2[] = "IsobusDriver v09.01.br03";

static const iso_char productIdentCode[] = "CCI A3 part number#0815";   /* 11783-7  -- A.22 Product Identification Code
                                                                                   1st ecuPartNumber
                                                                                   2nd ecuSerialNumber
                                                                                   from ECU Identification */
static const iso_char productIdentBrand[] = "CCI";                      /* 11783-7  -- A.23 Product Identification Brand */
static const iso_char productIdentModel[] = "CCI A3";                   /* 11783-7  -- A.24 Product Identification Model */
#endif
/* the following information can either be compiled based on the above information or used directly.
   Both versions are displayed below (see statement "#if(0)").
*/
static const iso_char ecuIdent[] = "CCI A3 part number*0815*Cabin*CCI A3*CCI e.V.*000209#A3 v.0.9*";
static const iso_char swIdent[] = "\x02AUX v0.1 20180217*IsobusDriver v09.01.br03*";
static const iso_char productIdent[] = "CCI A3 part number#0815*CCI*CCI A3*";
#if(0)
static const iso_u8 functionality[] = { 0xFF,    2,  MINIMUM_CF, 1, 0,   6, 2, 0, 0}; // graphical AUX-N (A3) 
#endif

// The following information is provided by  certification request from certification lab (from AEF database)
static const iso_u8  complianceTestProtocolRevision = 5;            /* 11783-7  -- A.29.2 -- 5 bits */
static const iso_u8  complianceTestProtocolPublicationYear = 17;    /* 11783-7  -- A.29.1 -- 6 bits */
static const iso_u8  complianceCertificationLabType = 1;            /* 11783-7  -- A.29.3 -- EU AEF certified lab */
static const iso_u16 complianceCertificationLabID = 507;            /* 11783-7  -- A.29.4 */
static const iso_u8  complianceCertificationMessageRevision = 1;    /* 11783-7  -- A.29.19 */
static const iso_u16 complianceCertificationReferenceNumber = 5131; /* 11783-7  -- A.29.18 */

/*  the following functions return individual values for CF's within a device */
static iso_u8* getFuncChar(enum CFType cfType, iso_u16* length);

/*  the following functions return identical values across all CF's within a device */
static iso_u8* getECUIdentification(iso_u16* length);
static iso_u8* getSoftwareIdentification(iso_u16* length);
static iso_u8* getProduxtIdentification(iso_u16* length);
static iso_u8* getComplianceCertificate(iso_u16* length);

/* the following functions return default values */
static iso_u8* getNoneActiveFaults(iso_u16* length);
static iso_u8* getNoneAdditionalDiagnostics(iso_u16* length);

iso_bool processPart12PGN(ISO_TPREP_E eTpRep, const ISO_TPINFO_T* psMsgInfo)
{
    iso_bool bRet = ISO_FALSE;

    if ((eTpRep == record_request) && (psMsgInfo != 0))
    {
        iso_u16 identSize = 0;
        iso_u8* ident = 0;

        switch (psMsgInfo->dwPGN)
        {
        case PGN_ECU_IDENTIFICATION_INFO:
            ident = getECUIdentification(&identSize);
            break;

        case PGN_SOFTWARE_IDENTIFICATION:
            /* ECU software identification */
            ident = getSoftwareIdentification(&identSize);
            break;

        case PGN_PRODUCT_IDENTIFICATION:
            /* Produkt identification message */
            ident = getProduxtIdentification(&identSize);
            break;

        case PGN_ECU_DIAGNOSTIC_PROTOCOL:
            /* ECU diagnostic protocol */
            ident = getNoneAdditionalDiagnostics(&identSize);
            break;

#if (0) /* Functionalities are CF specific
         * identify by psMsgInfo->s16HndIntern and use here;
         * or e.g. handle using iso_AlPgnTxNew()
         */
        case PGN_FUNCTIONALI_CHARACTERISTICS:
            /* Functionality message */
            ident = getFuncChar(CFTypeIsInvalid, &identSize);
            break;
#endif
        case PGN_ISOBUS_COMPLIANCE_CERTIFICA:
            /* ISOBus compliance cerfification */
            ident = getComplianceCertificate(&identSize);
            break;

        /* J1939-73 / 11783-12, B.x - Application layer - Diagnostics */

        case PGN_ACTIVE_DIAG_TROUBLE_CODES:
            // DM1 message
            ident = getNoneActiveFaults(&identSize);
            break;

        case PGN_PREV_ACTIV_DIAG_TROUBLE_COD:
            // DM2 message
            ident = getNoneActiveFaults(&identSize);
            break;

        default:
            break;
        }

        if ((ident != 0) && (identSize > 0))
        {
            iso_s16 s16Ret;
            s16Ret = iso_DlPgnSend(6, psMsgInfo->dwPGN, identSize, ident,
                                   psMsgInfo->s16HndIntern, psMsgInfo->s16HndPartner);
            (void)s16Ret;
            bRet = ISO_TRUE;
        }
    }

    return bRet;
}

void installPart12PGN(enum CFType cfType, iso_s16 nmHandle)
{
#if defined (_LAY78_) /* Part 7 - Implement messages application layer module is configured */
   ISO_USER_PARAM_T userParam = ISO_USER_PARAM_DEFAULT;
    iso_s16 pgnHandle = HANDLE_UNVALID;
    iso_u16 identSize = 0;
    iso_u8* ident = 0;
/* some entries are identical across all CF's within a device. others are unique to a CF */

/*  11783-12 B.8 diagnostic data clear DM3 */
/* PGN_DIAGNOSTIC_DATA_CLEAR needs to be handled by application! */

    ident = getECUIdentification(&identSize);
    pgnHandle = iso_AlPgnTxNew(nmHandle,
        PGN_ECU_IDENTIFICATION_INFO,
        ISO_GLOBAL_ADDRESS,
        identSize, ident, 6, REPRATE_INACTIVE, userParam, 0);
    iso_AlPgnActivate(pgnHandle);

    /* ECU software identification */
    ident = getSoftwareIdentification(&identSize);
    pgnHandle = iso_AlPgnTxNew(nmHandle,
        PGN_SOFTWARE_IDENTIFICATION,
        ISO_GLOBAL_ADDRESS,
        identSize, ident, 6, REPRATE_INACTIVE, userParam, 0);
    iso_AlPgnActivate(pgnHandle);

    /* Produkt identification message */
    ident = getProduxtIdentification(&identSize);
    pgnHandle = iso_AlPgnTxNew(nmHandle,
        PGN_PRODUCT_IDENTIFICATION,
        ISO_GLOBAL_ADDRESS,
        identSize, ident, 6, REPRATE_INACTIVE, userParam, 0);
    iso_AlPgnActivate(pgnHandle);

    /* ECU diagnostic protocol */
    ident = getNoneAdditionalDiagnostics(&identSize);
    pgnHandle = iso_AlPgnTxNew(nmHandle,
        PGN_ECU_DIAGNOSTIC_PROTOCOL,
        ISO_GLOBAL_ADDRESS,
        identSize, ident, 6, REPRATE_INACTIVE, userParam, 0);
    iso_AlPgnActivate(pgnHandle);

    /* Functionality message */
    ident = getFuncChar(cfType, &identSize);
    pgnHandle = iso_AlPgnTxNew(nmHandle,
        PGN_FUNCTIONALI_CHARACTERISTICS,
        ISO_GLOBAL_ADDRESS,
        identSize, ident, 6, REPRATE_INACTIVE, userParam, 0);
    iso_AlPgnActivate(pgnHandle);

    /* ISOBus compliance cerfification */
    ident = getComplianceCertificate(&identSize);
    pgnHandle = iso_AlPgnTxNew(nmHandle,
        PGN_ISOBUS_COMPLIANCE_CERTIFICA,
        ISO_GLOBAL_ADDRESS,
        identSize, ident, 6, REPRATE_INACTIVE, userParam, 0);
    iso_AlPgnActivate(pgnHandle);

/* J1939-73 / 11783-12, B.x - Application layer - Diagnostics */

    // DM1 message 
    ident = getNoneActiveFaults(&identSize);
    pgnHandle = iso_AlPgnTxNew(nmHandle,
        PGN_ACTIVE_DIAG_TROUBLE_CODES,
        ISO_GLOBAL_ADDRESS,
        identSize, ident, 6, REPRATE_INACTIVE, userParam, 0);
    iso_AlPgnActivate(pgnHandle);

    // DM2 message 
    ident = getNoneActiveFaults(&identSize);
    pgnHandle = iso_AlPgnTxNew(nmHandle,
        PGN_PREV_ACTIV_DIAG_TROUBLE_COD,
        ISO_GLOBAL_ADDRESS,
        identSize, ident, 6, REPRATE_INACTIVE, userParam, 0);
    iso_AlPgnActivate(pgnHandle);
#endif /* defined (_LAY78_) */

}

iso_u8* getECUIdentification(iso_u16* length)
{
/*  ECU identification information */
/*  11783-12 B.1 ECU identification information */
/*  identical across all CF's within a device */
#if(0)
    static char ecuIdent[sizeof(ecuPartNumber) + 
                         sizeof(ecuSerialNumber) + 
                         sizeof(ecuLocation) + 
                         sizeof(ecuType) + 
                         sizeof(ecuManufacturerName) + 
                         sizeof(ecuHardwareVersionId) + 
                         1] = {0};
    int tempLength = sprintf(&ecuIdent[0], "%s*%s*%s*%s*%s*%s*", 
                             ecuPartNumber, ecuSerialNumber, ecuLocation,
                             ecuType, ecuManufacturerName, ecuHardwareVersionId);
#else
    int tempLength = sizeof(ecuIdent) - 1;
#endif

    if (length != 0)
    {
        *length = tempLength;
    }

    return (iso_u8*)ecuIdent;
}

iso_u8* getSoftwareIdentification(iso_u16* length)
{
/*  first byte is the number of software identification fields */
/*  11783-12 B.2 Software identification */                  
/*  identical across all CF's within a device */
#if(0)
    const char identCount = '\x02';                          /*  11783-12 -- A.3 number of software identification fileds */
    static char swIdent[sizeof(swId1) + sizeof(swId2) + 2] = {0};
    int tempLength = sprintf(&swIdent[0], "%c%s*%s*", identCount, swId1, swId2);
#else
    int tempLength = sizeof(swIdent) - 1;
#endif

    if (length != 0)
    {
        *length = tempLength;
    }

    return (iso_u8*)swIdent;
}

iso_u8* getProduxtIdentification(iso_u16* length)
{
/*  11783-12 B.10 product identification*/
/* identical across all CF's within a device */
/* Product Identification Code  A.22 */
/* Product Identification Brand A.23 */
/* Product Identification Model A.24 */
#if(0)
    static char productIdent[sizeof(productIdentCode) + 
                             sizeof(productIdentBrand) + 
                             sizeof(productIdentModel) + 
                         1] = {0};
    
    int tempLength = sprintf(&productIdent[0], "%s*%s*%s*", 
                             productIdentCode, productIdentBrand, productIdentModel);
#else
    int tempLength = sizeof(productIdent) - 1;
#endif

    if (length != 0)
    {
        *length = tempLength;
    }

    return (iso_u8*)productIdent;
}

iso_u8* getComplianceCertificate(iso_u16* length)
{
/*  11783-7 B.27 compliance certification message */
/*  identical across all CF's within a device */
/*  refer to part 7 for details */
    static iso_u8 au8ComCert[8] = {0};
    au8ComCert[0] = ((complianceTestProtocolRevision & 0x3) << 6) | 
                    (complianceTestProtocolPublicationYear & 0x3F);
    au8ComCert[1] = ((complianceCertificationLabID & 0x7) << 5) | 
                    (complianceTestProtocolRevision & 0x18) | 
                    ((complianceCertificationLabType & 0x03) << 1) | 
                    ((complianceTestProtocolRevision & 0x04) >> 2);
    au8ComCert[2] = ((complianceCertificationLabID & 0x7F8) >> 3);
    au8ComCert[5] = ((complianceCertificationMessageRevision & 0x1) << 7);
    au8ComCert[6] = (complianceCertificationReferenceNumber & 0xFF);
    au8ComCert[7] = ((complianceCertificationReferenceNumber & 0xFF00) >> 8);

    if (length != 0)
    {
        *length = sizeof(au8ComCert);
    }

    return au8ComCert;
}

static iso_u8* getFuncChar(enum CFType cfType, iso_u16* length)
{
/*  11783-12 B.9 control function characteristics */
/*  Byte 1: FF */
/*  Byte 2:   number of functionalities reported in this message A.9 */
/*  Byte 3:   1st functionality  A.10 */
/*  Byte 4:   1st functionality  A.11 generation */
/*  Byte 5:   1st functionality, A.12 number of option bytes */
/*  ...       2nd functionality, A.13 option bytes */
/*  Byte n:   2nd functionality  A.10 */
/*  Byte n+1: 2nd functionality  A.11 generation */
/*  Byte n+2: 2nd functionality, A.12 number of option bytes */
/*  ...       2nd functionality, A.13 option bytes */
/* ... */
    static iso_u8 au8FuncCharAuxN[] = { 0xFF,  
                                        2,  
                                        MINIMUM_CF, 1, 0, /* this is typically always the first entry */
                                        6, 2, 0, 0};      /* AUX-N, no optional functions */

    iso_u8* ident = 0;
    iso_u16 tempLength = 0;

    switch(cfType)
    {
    case CFTypeIsAuxN:
        ident = au8FuncCharAuxN;
        tempLength = sizeof(au8FuncCharAuxN);
        break;

    default:
        break;
    }

    if (length != 0)
    {
        *length = tempLength;
    }

    return ident;
}

static iso_u8* getNoneActiveFaults(iso_u16* length)
{
    static iso_u8 au8DMx[8] = { 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF };

    if (length != 0)
    {
        *length = sizeof(au8DMx);
    }

    return au8DMx;
}

static iso_u8* getNoneAdditionalDiagnostics(iso_u16* length)
{
/*  11783-12 B.5 diagnostic protocol */
/*  Byte 1 Diagnostic Protocol Information */              /*  11783-12 -- A.6 Diagnostic Protocol Information; 
                                                                               0: no additional diagnostics */
/*  Byte 2 - 8 reserved */
    static iso_u8 au8DiagPro[] = { 0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

    if (length != 0)
    {
        *length = sizeof(au8DiagPro);
    }

    return au8DiagPro;
}

/* ************************************************************************ */

// Copyright 2015 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#include "JBig2_SddProc.h"

#include "../../../../third_party/base/nonstd_unique_ptr.h"
#include "../../../../third_party/base/stl_util.h"
#include "../../../include/fxcrt/fx_basic.h"
#include "JBig2_ArithIntDecoder.h"
#include "JBig2_GrdProc.h"
#include "JBig2_GrrdProc.h"
#include "JBig2_HuffmanDecoder.h"
#include "JBig2_HuffmanTable.h"
#include "JBig2_SymbolDict.h"
#include "JBig2_TrdProc.h"

using pdfium::vector_as_array;

CJBig2_SymbolDict* CJBig2_SDDProc::decode_Arith(
    CJBig2_ArithDecoder* pArithDecoder,
    std::vector<JBig2ArithCtx>* gbContext,
    std::vector<JBig2ArithCtx>* grContext) {
  CJBig2_Image** SDNEWSYMS;
  FX_DWORD HCHEIGHT, NSYMSDECODED;
  int32_t HCDH;
  FX_DWORD SYMWIDTH, TOTWIDTH;
  int32_t DW;
  CJBig2_Image* BS;
  FX_DWORD I, J, REFAGGNINST;
  FX_BOOL* EXFLAGS;
  FX_DWORD EXINDEX;
  FX_BOOL CUREXFLAG;
  FX_DWORD EXRUNLENGTH;
  FX_DWORD nTmp;
  FX_DWORD SBNUMSYMS;
  uint8_t SBSYMCODELEN;
  int32_t RDXI, RDYI;
  CJBig2_Image** SBSYMS;
  nonstd::unique_ptr<CJBig2_ArithIaidDecoder> IAID;
  nonstd::unique_ptr<CJBig2_SymbolDict> pDict;
  nonstd::unique_ptr<CJBig2_ArithIntDecoder> IADH(new CJBig2_ArithIntDecoder);
  nonstd::unique_ptr<CJBig2_ArithIntDecoder> IADW(new CJBig2_ArithIntDecoder);
  nonstd::unique_ptr<CJBig2_ArithIntDecoder> IAAI(new CJBig2_ArithIntDecoder);
  nonstd::unique_ptr<CJBig2_ArithIntDecoder> IARDX(new CJBig2_ArithIntDecoder);
  nonstd::unique_ptr<CJBig2_ArithIntDecoder> IARDY(new CJBig2_ArithIntDecoder);
  nonstd::unique_ptr<CJBig2_ArithIntDecoder> IAEX(new CJBig2_ArithIntDecoder);
  nonstd::unique_ptr<CJBig2_ArithIntDecoder> IADT(new CJBig2_ArithIntDecoder);
  nonstd::unique_ptr<CJBig2_ArithIntDecoder> IAFS(new CJBig2_ArithIntDecoder);
  nonstd::unique_ptr<CJBig2_ArithIntDecoder> IADS(new CJBig2_ArithIntDecoder);
  nonstd::unique_ptr<CJBig2_ArithIntDecoder> IAIT(new CJBig2_ArithIntDecoder);
  nonstd::unique_ptr<CJBig2_ArithIntDecoder> IARI(new CJBig2_ArithIntDecoder);
  nonstd::unique_ptr<CJBig2_ArithIntDecoder> IARDW(new CJBig2_ArithIntDecoder);
  nonstd::unique_ptr<CJBig2_ArithIntDecoder> IARDH(new CJBig2_ArithIntDecoder);
  nTmp = 0;
  while ((FX_DWORD)(1 << nTmp) < (SDNUMINSYMS + SDNUMNEWSYMS)) {
    nTmp++;
  }
  IAID.reset(new CJBig2_ArithIaidDecoder((uint8_t)nTmp));
  SDNEWSYMS = FX_Alloc(CJBig2_Image*, SDNUMNEWSYMS);
  FXSYS_memset(SDNEWSYMS, 0, SDNUMNEWSYMS * sizeof(CJBig2_Image*));

  HCHEIGHT = 0;
  NSYMSDECODED = 0;
  while (NSYMSDECODED < SDNUMNEWSYMS) {
    BS = nullptr;
    IADH->decode(pArithDecoder, &HCDH);
    HCHEIGHT = HCHEIGHT + HCDH;
    if ((int)HCHEIGHT < 0 || (int)HCHEIGHT > JBIG2_MAX_IMAGE_SIZE) {
      goto failed;
    }
    SYMWIDTH = 0;
    TOTWIDTH = 0;
    for (;;) {
      if (!IADW->decode(pArithDecoder, &DW))
        break;

      if (NSYMSDECODED >= SDNUMNEWSYMS)
        goto failed;

      SYMWIDTH = SYMWIDTH + DW;
      if ((int)SYMWIDTH < 0 || (int)SYMWIDTH > JBIG2_MAX_IMAGE_SIZE)
        goto failed;

      if (HCHEIGHT == 0 || SYMWIDTH == 0) {
        TOTWIDTH = TOTWIDTH + SYMWIDTH;
        SDNEWSYMS[NSYMSDECODED] = nullptr;
        NSYMSDECODED = NSYMSDECODED + 1;
        continue;
      }
      TOTWIDTH = TOTWIDTH + SYMWIDTH;
      if (SDREFAGG == 0) {
        nonstd::unique_ptr<CJBig2_GRDProc> pGRD(new CJBig2_GRDProc());
        pGRD->MMR = 0;
        pGRD->GBW = SYMWIDTH;
        pGRD->GBH = HCHEIGHT;
        pGRD->GBTEMPLATE = SDTEMPLATE;
        pGRD->TPGDON = 0;
        pGRD->USESKIP = 0;
        pGRD->GBAT[0] = SDAT[0];
        pGRD->GBAT[1] = SDAT[1];
        pGRD->GBAT[2] = SDAT[2];
        pGRD->GBAT[3] = SDAT[3];
        pGRD->GBAT[4] = SDAT[4];
        pGRD->GBAT[5] = SDAT[5];
        pGRD->GBAT[6] = SDAT[6];
        pGRD->GBAT[7] = SDAT[7];
        BS = pGRD->decode_Arith(pArithDecoder, vector_as_array(gbContext));
        if (!BS) {
          goto failed;
        }
      } else {
        IAAI->decode(pArithDecoder, (int*)&REFAGGNINST);
        if (REFAGGNINST > 1) {
          nonstd::unique_ptr<CJBig2_TRDProc> pDecoder(new CJBig2_TRDProc());
          pDecoder->SBHUFF = SDHUFF;
          pDecoder->SBREFINE = 1;
          pDecoder->SBW = SYMWIDTH;
          pDecoder->SBH = HCHEIGHT;
          pDecoder->SBNUMINSTANCES = REFAGGNINST;
          pDecoder->SBSTRIPS = 1;
          pDecoder->SBNUMSYMS = SDNUMINSYMS + NSYMSDECODED;
          SBNUMSYMS = pDecoder->SBNUMSYMS;
          nTmp = 0;
          while ((FX_DWORD)(1 << nTmp) < SBNUMSYMS) {
            nTmp++;
          }
          SBSYMCODELEN = (uint8_t)nTmp;
          pDecoder->SBSYMCODELEN = SBSYMCODELEN;
          SBSYMS = FX_Alloc(CJBig2_Image*, SBNUMSYMS);
          JBIG2_memcpy(SBSYMS, SDINSYMS, SDNUMINSYMS * sizeof(CJBig2_Image*));
          JBIG2_memcpy(SBSYMS + SDNUMINSYMS, SDNEWSYMS,
                       NSYMSDECODED * sizeof(CJBig2_Image*));
          pDecoder->SBSYMS = SBSYMS;
          pDecoder->SBDEFPIXEL = 0;
          pDecoder->SBCOMBOP = JBIG2_COMPOSE_OR;
          pDecoder->TRANSPOSED = 0;
          pDecoder->REFCORNER = JBIG2_CORNER_TOPLEFT;
          pDecoder->SBDSOFFSET = 0;
          nonstd::unique_ptr<CJBig2_HuffmanTable> SBHUFFFS(
              new CJBig2_HuffmanTable(HuffmanTable_B6,
                                      FX_ArraySize(HuffmanTable_B6),
                                      HuffmanTable_HTOOB_B6));
          nonstd::unique_ptr<CJBig2_HuffmanTable> SBHUFFDS(
              new CJBig2_HuffmanTable(HuffmanTable_B8,
                                      FX_ArraySize(HuffmanTable_B8),
                                      HuffmanTable_HTOOB_B8));
          nonstd::unique_ptr<CJBig2_HuffmanTable> SBHUFFDT(
              new CJBig2_HuffmanTable(HuffmanTable_B11,
                                      FX_ArraySize(HuffmanTable_B11),
                                      HuffmanTable_HTOOB_B11));
          nonstd::unique_ptr<CJBig2_HuffmanTable> SBHUFFRDW(
              new CJBig2_HuffmanTable(HuffmanTable_B15,
                                      FX_ArraySize(HuffmanTable_B15),
                                      HuffmanTable_HTOOB_B15));
          nonstd::unique_ptr<CJBig2_HuffmanTable> SBHUFFRDH(
              new CJBig2_HuffmanTable(HuffmanTable_B15,
                                      FX_ArraySize(HuffmanTable_B15),
                                      HuffmanTable_HTOOB_B15));
          nonstd::unique_ptr<CJBig2_HuffmanTable> SBHUFFRDX(
              new CJBig2_HuffmanTable(HuffmanTable_B15,
                                      FX_ArraySize(HuffmanTable_B15),
                                      HuffmanTable_HTOOB_B15));
          nonstd::unique_ptr<CJBig2_HuffmanTable> SBHUFFRDY(
              new CJBig2_HuffmanTable(HuffmanTable_B15,
                                      FX_ArraySize(HuffmanTable_B15),
                                      HuffmanTable_HTOOB_B15));
          nonstd::unique_ptr<CJBig2_HuffmanTable> SBHUFFRSIZE(
              new CJBig2_HuffmanTable(HuffmanTable_B1,
                                      FX_ArraySize(HuffmanTable_B1),
                                      HuffmanTable_HTOOB_B1));
          pDecoder->SBHUFFFS = SBHUFFFS.get();
          pDecoder->SBHUFFDS = SBHUFFDS.get();
          pDecoder->SBHUFFDT = SBHUFFDT.get();
          pDecoder->SBHUFFRDW = SBHUFFRDW.get();
          pDecoder->SBHUFFRDH = SBHUFFRDH.get();
          pDecoder->SBHUFFRDX = SBHUFFRDX.get();
          pDecoder->SBHUFFRDY = SBHUFFRDY.get();
          pDecoder->SBHUFFRSIZE = SBHUFFRSIZE.get();
          pDecoder->SBRTEMPLATE = SDRTEMPLATE;
          pDecoder->SBRAT[0] = SDRAT[0];
          pDecoder->SBRAT[1] = SDRAT[1];
          pDecoder->SBRAT[2] = SDRAT[2];
          pDecoder->SBRAT[3] = SDRAT[3];
          JBig2IntDecoderState ids;
          ids.IADT = IADT.get();
          ids.IAFS = IAFS.get();
          ids.IADS = IADS.get();
          ids.IAIT = IAIT.get();
          ids.IARI = IARI.get();
          ids.IARDW = IARDW.get();
          ids.IARDH = IARDH.get();
          ids.IARDX = IARDX.get();
          ids.IARDY = IARDY.get();
          ids.IAID = IAID.get();
          BS = pDecoder->decode_Arith(pArithDecoder, vector_as_array(grContext),
                                      &ids);
          if (!BS) {
            FX_Free(SBSYMS);
            goto failed;
          }
          FX_Free(SBSYMS);
        } else if (REFAGGNINST == 1) {
          SBNUMSYMS = SDNUMINSYMS + NSYMSDECODED;
          FX_DWORD IDI;
          IAID->decode(pArithDecoder, &IDI);
          IARDX->decode(pArithDecoder, &RDXI);
          IARDY->decode(pArithDecoder, &RDYI);
          if (IDI >= SBNUMSYMS) {
            goto failed;
          }
          SBSYMS = FX_Alloc(CJBig2_Image*, SBNUMSYMS);
          JBIG2_memcpy(SBSYMS, SDINSYMS, SDNUMINSYMS * sizeof(CJBig2_Image*));
          JBIG2_memcpy(SBSYMS + SDNUMINSYMS, SDNEWSYMS,
                       NSYMSDECODED * sizeof(CJBig2_Image*));
          if (!SBSYMS[IDI]) {
            FX_Free(SBSYMS);
            goto failed;
          }
          nonstd::unique_ptr<CJBig2_GRRDProc> pGRRD(new CJBig2_GRRDProc());
          pGRRD->GRW = SYMWIDTH;
          pGRRD->GRH = HCHEIGHT;
          pGRRD->GRTEMPLATE = SDRTEMPLATE;
          pGRRD->GRREFERENCE = SBSYMS[IDI];
          pGRRD->GRREFERENCEDX = RDXI;
          pGRRD->GRREFERENCEDY = RDYI;
          pGRRD->TPGRON = 0;
          pGRRD->GRAT[0] = SDRAT[0];
          pGRRD->GRAT[1] = SDRAT[1];
          pGRRD->GRAT[2] = SDRAT[2];
          pGRRD->GRAT[3] = SDRAT[3];
          BS = pGRRD->decode(pArithDecoder, vector_as_array(grContext));
          if (!BS) {
            FX_Free(SBSYMS);
            goto failed;
          }
          FX_Free(SBSYMS);
        }
      }
      SDNEWSYMS[NSYMSDECODED] = BS;
      BS = nullptr;
      NSYMSDECODED = NSYMSDECODED + 1;
    }
  }
  EXINDEX = 0;
  CUREXFLAG = 0;
  EXFLAGS = FX_Alloc(FX_BOOL, SDNUMINSYMS + SDNUMNEWSYMS);
  while (EXINDEX < SDNUMINSYMS + SDNUMNEWSYMS) {
    IAEX->decode(pArithDecoder, (int*)&EXRUNLENGTH);
    if (EXINDEX + EXRUNLENGTH > SDNUMINSYMS + SDNUMNEWSYMS) {
      FX_Free(EXFLAGS);
      goto failed;
    }
    if (EXRUNLENGTH != 0) {
      for (I = EXINDEX; I < EXINDEX + EXRUNLENGTH; I++) {
        EXFLAGS[I] = CUREXFLAG;
      }
    }
    EXINDEX = EXINDEX + EXRUNLENGTH;
    CUREXFLAG = !CUREXFLAG;
  }
  pDict.reset(new CJBig2_SymbolDict);
  I = J = 0;
  for (I = 0; I < SDNUMINSYMS + SDNUMNEWSYMS; I++) {
    if (EXFLAGS[I] && J < SDNUMEXSYMS) {
      if (I < SDNUMINSYMS) {
        pDict->AddImage(new CJBig2_Image(*SDINSYMS[I]));
      } else {
        pDict->AddImage(SDNEWSYMS[I - SDNUMINSYMS]);
      }
      ++J;
    } else if (!EXFLAGS[I] && I >= SDNUMINSYMS) {
      delete SDNEWSYMS[I - SDNUMINSYMS];
    }
  }
  FX_Free(EXFLAGS);
  FX_Free(SDNEWSYMS);
  return pDict.release();
failed:
  for (I = 0; I < NSYMSDECODED; I++) {
    if (SDNEWSYMS[I]) {
      delete SDNEWSYMS[I];
      SDNEWSYMS[I] = nullptr;
    }
  }
  FX_Free(SDNEWSYMS);
  return nullptr;
}

CJBig2_SymbolDict* CJBig2_SDDProc::decode_Huffman(
    CJBig2_BitStream* pStream,
    std::vector<JBig2ArithCtx>* gbContext,
    std::vector<JBig2ArithCtx>* grContext,
    IFX_Pause* pPause) {
  CJBig2_Image** SDNEWSYMS;
  FX_DWORD* SDNEWSYMWIDTHS;
  FX_DWORD HCHEIGHT, NSYMSDECODED;
  int32_t HCDH;
  FX_DWORD SYMWIDTH, TOTWIDTH, HCFIRSTSYM;
  int32_t DW;
  CJBig2_Image* BS, *BHC;
  FX_DWORD I, J, REFAGGNINST;
  FX_BOOL* EXFLAGS;
  FX_DWORD EXINDEX;
  FX_BOOL CUREXFLAG;
  FX_DWORD EXRUNLENGTH;
  int32_t nVal, nBits;
  FX_DWORD nTmp;
  FX_DWORD SBNUMSYMS;
  uint8_t SBSYMCODELEN;
  JBig2HuffmanCode* SBSYMCODES;
  FX_DWORD IDI;
  int32_t RDXI, RDYI;
  FX_DWORD BMSIZE;
  FX_DWORD stride;
  CJBig2_Image** SBSYMS;
  nonstd::unique_ptr<CJBig2_HuffmanDecoder> pHuffmanDecoder(
      new CJBig2_HuffmanDecoder(pStream));
  SDNEWSYMS = FX_Alloc(CJBig2_Image*, SDNUMNEWSYMS);
  FXSYS_memset(SDNEWSYMS, 0, SDNUMNEWSYMS * sizeof(CJBig2_Image*));
  SDNEWSYMWIDTHS = nullptr;
  BHC = nullptr;
  if (SDREFAGG == 0) {
    SDNEWSYMWIDTHS = FX_Alloc(FX_DWORD, SDNUMNEWSYMS);
    FXSYS_memset(SDNEWSYMWIDTHS, 0, SDNUMNEWSYMS * sizeof(FX_DWORD));
  }
  nonstd::unique_ptr<CJBig2_SymbolDict> pDict(new CJBig2_SymbolDict());
  nonstd::unique_ptr<CJBig2_HuffmanTable> pTable;

  HCHEIGHT = 0;
  NSYMSDECODED = 0;
  BS = nullptr;
  while (NSYMSDECODED < SDNUMNEWSYMS) {
    if (pHuffmanDecoder->decodeAValue(SDHUFFDH, &HCDH) != 0) {
      goto failed;
    }
    HCHEIGHT = HCHEIGHT + HCDH;
    if ((int)HCHEIGHT < 0 || (int)HCHEIGHT > JBIG2_MAX_IMAGE_SIZE) {
      goto failed;
    }
    SYMWIDTH = 0;
    TOTWIDTH = 0;
    HCFIRSTSYM = NSYMSDECODED;
    for (;;) {
      nVal = pHuffmanDecoder->decodeAValue(SDHUFFDW, &DW);
      if (nVal == JBIG2_OOB) {
        break;
      } else if (nVal != 0) {
        goto failed;
      } else {
        if (NSYMSDECODED >= SDNUMNEWSYMS) {
          goto failed;
        }
        SYMWIDTH = SYMWIDTH + DW;
        if ((int)SYMWIDTH < 0 || (int)SYMWIDTH > JBIG2_MAX_IMAGE_SIZE) {
          goto failed;
        } else if (HCHEIGHT == 0 || SYMWIDTH == 0) {
          TOTWIDTH = TOTWIDTH + SYMWIDTH;
          SDNEWSYMS[NSYMSDECODED] = nullptr;
          NSYMSDECODED = NSYMSDECODED + 1;
          continue;
        }
        TOTWIDTH = TOTWIDTH + SYMWIDTH;
      }
      if (SDREFAGG == 1) {
        if (pHuffmanDecoder->decodeAValue(SDHUFFAGGINST, (int*)&REFAGGNINST) !=
            0) {
          goto failed;
        }
        BS = nullptr;
        if (REFAGGNINST > 1) {
          nonstd::unique_ptr<CJBig2_TRDProc> pDecoder(new CJBig2_TRDProc());
          pDecoder->SBHUFF = SDHUFF;
          pDecoder->SBREFINE = 1;
          pDecoder->SBW = SYMWIDTH;
          pDecoder->SBH = HCHEIGHT;
          pDecoder->SBNUMINSTANCES = REFAGGNINST;
          pDecoder->SBSTRIPS = 1;
          pDecoder->SBNUMSYMS = SDNUMINSYMS + NSYMSDECODED;
          SBNUMSYMS = pDecoder->SBNUMSYMS;
          SBSYMCODES = FX_Alloc(JBig2HuffmanCode, SBNUMSYMS);
          nTmp = 1;
          while ((FX_DWORD)(1 << nTmp) < SBNUMSYMS) {
            nTmp++;
          }
          for (I = 0; I < SBNUMSYMS; I++) {
            SBSYMCODES[I].codelen = nTmp;
            SBSYMCODES[I].code = I;
          }
          pDecoder->SBSYMCODES = SBSYMCODES;
          SBSYMS = FX_Alloc(CJBig2_Image*, SBNUMSYMS);
          JBIG2_memcpy(SBSYMS, SDINSYMS, SDNUMINSYMS * sizeof(CJBig2_Image*));
          JBIG2_memcpy(SBSYMS + SDNUMINSYMS, SDNEWSYMS,
                       NSYMSDECODED * sizeof(CJBig2_Image*));
          pDecoder->SBSYMS = SBSYMS;
          pDecoder->SBDEFPIXEL = 0;
          pDecoder->SBCOMBOP = JBIG2_COMPOSE_OR;
          pDecoder->TRANSPOSED = 0;
          pDecoder->REFCORNER = JBIG2_CORNER_TOPLEFT;
          pDecoder->SBDSOFFSET = 0;
          nonstd::unique_ptr<CJBig2_HuffmanTable> SBHUFFFS(
              new CJBig2_HuffmanTable(HuffmanTable_B6,
                                      FX_ArraySize(HuffmanTable_B6),
                                      HuffmanTable_HTOOB_B6));
          nonstd::unique_ptr<CJBig2_HuffmanTable> SBHUFFDS(
              new CJBig2_HuffmanTable(HuffmanTable_B8,
                                      FX_ArraySize(HuffmanTable_B8),
                                      HuffmanTable_HTOOB_B8));
          nonstd::unique_ptr<CJBig2_HuffmanTable> SBHUFFDT(
              new CJBig2_HuffmanTable(HuffmanTable_B11,
                                      FX_ArraySize(HuffmanTable_B11),
                                      HuffmanTable_HTOOB_B11));
          nonstd::unique_ptr<CJBig2_HuffmanTable> SBHUFFRDW(
              new CJBig2_HuffmanTable(HuffmanTable_B15,
                                      FX_ArraySize(HuffmanTable_B15),
                                      HuffmanTable_HTOOB_B15));
          nonstd::unique_ptr<CJBig2_HuffmanTable> SBHUFFRDH(
              new CJBig2_HuffmanTable(HuffmanTable_B15,
                                      FX_ArraySize(HuffmanTable_B15),
                                      HuffmanTable_HTOOB_B15));
          nonstd::unique_ptr<CJBig2_HuffmanTable> SBHUFFRDX(
              new CJBig2_HuffmanTable(HuffmanTable_B15,
                                      FX_ArraySize(HuffmanTable_B15),
                                      HuffmanTable_HTOOB_B15));
          nonstd::unique_ptr<CJBig2_HuffmanTable> SBHUFFRDY(
              new CJBig2_HuffmanTable(HuffmanTable_B15,
                                      FX_ArraySize(HuffmanTable_B15),
                                      HuffmanTable_HTOOB_B15));
          nonstd::unique_ptr<CJBig2_HuffmanTable> SBHUFFRSIZE(
              new CJBig2_HuffmanTable(HuffmanTable_B1,
                                      FX_ArraySize(HuffmanTable_B1),
                                      HuffmanTable_HTOOB_B1));
          pDecoder->SBHUFFFS = SBHUFFFS.get();
          pDecoder->SBHUFFDS = SBHUFFDS.get();
          pDecoder->SBHUFFDT = SBHUFFDT.get();
          pDecoder->SBHUFFRDW = SBHUFFRDW.get();
          pDecoder->SBHUFFRDH = SBHUFFRDH.get();
          pDecoder->SBHUFFRDX = SBHUFFRDX.get();
          pDecoder->SBHUFFRDY = SBHUFFRDY.get();
          pDecoder->SBHUFFRSIZE = SBHUFFRSIZE.get();
          pDecoder->SBRTEMPLATE = SDRTEMPLATE;
          pDecoder->SBRAT[0] = SDRAT[0];
          pDecoder->SBRAT[1] = SDRAT[1];
          pDecoder->SBRAT[2] = SDRAT[2];
          pDecoder->SBRAT[3] = SDRAT[3];
          BS = pDecoder->decode_Huffman(pStream, vector_as_array(grContext));
          if (!BS) {
            FX_Free(SBSYMCODES);
            FX_Free(SBSYMS);
            goto failed;
          }
          FX_Free(SBSYMCODES);
          FX_Free(SBSYMS);
        } else if (REFAGGNINST == 1) {
          SBNUMSYMS = SDNUMINSYMS + SDNUMNEWSYMS;
          nTmp = 1;
          while ((FX_DWORD)(1 << nTmp) < SBNUMSYMS) {
            nTmp++;
          }
          SBSYMCODELEN = (uint8_t)nTmp;
          SBSYMCODES = FX_Alloc(JBig2HuffmanCode, SBNUMSYMS);
          for (I = 0; I < SBNUMSYMS; I++) {
            SBSYMCODES[I].codelen = SBSYMCODELEN;
            SBSYMCODES[I].code = I;
          }
          nVal = 0;
          nBits = 0;
          for (;;) {
            if (pStream->read1Bit(&nTmp) != 0) {
              FX_Free(SBSYMCODES);
              goto failed;
            }
            nVal = (nVal << 1) | nTmp;
            for (IDI = 0; IDI < SBNUMSYMS; IDI++) {
              if ((nVal == SBSYMCODES[IDI].code) &&
                  (nBits == SBSYMCODES[IDI].codelen)) {
                break;
              }
            }
            if (IDI < SBNUMSYMS) {
              break;
            }
          }
          FX_Free(SBSYMCODES);
          nonstd::unique_ptr<CJBig2_HuffmanTable> SBHUFFRDX(
              new CJBig2_HuffmanTable(HuffmanTable_B15,
                                      FX_ArraySize(HuffmanTable_B15),
                                      HuffmanTable_HTOOB_B15));
          nonstd::unique_ptr<CJBig2_HuffmanTable> SBHUFFRSIZE(
              new CJBig2_HuffmanTable(HuffmanTable_B1,
                                      FX_ArraySize(HuffmanTable_B1),
                                      HuffmanTable_HTOOB_B1));
          if ((pHuffmanDecoder->decodeAValue(SBHUFFRDX.get(), &RDXI) != 0) ||
              (pHuffmanDecoder->decodeAValue(SBHUFFRDX.get(), &RDYI) != 0) ||
              (pHuffmanDecoder->decodeAValue(SBHUFFRSIZE.get(), &nVal) != 0)) {
            goto failed;
          }
          pStream->alignByte();
          nTmp = pStream->getOffset();
          SBSYMS = FX_Alloc(CJBig2_Image*, SBNUMSYMS);
          JBIG2_memcpy(SBSYMS, SDINSYMS, SDNUMINSYMS * sizeof(CJBig2_Image*));
          JBIG2_memcpy(SBSYMS + SDNUMINSYMS, SDNEWSYMS,
                       NSYMSDECODED * sizeof(CJBig2_Image*));
          nonstd::unique_ptr<CJBig2_GRRDProc> pGRRD(new CJBig2_GRRDProc());
          pGRRD->GRW = SYMWIDTH;
          pGRRD->GRH = HCHEIGHT;
          pGRRD->GRTEMPLATE = SDRTEMPLATE;
          pGRRD->GRREFERENCE = SBSYMS[IDI];
          pGRRD->GRREFERENCEDX = RDXI;
          pGRRD->GRREFERENCEDY = RDYI;
          pGRRD->TPGRON = 0;
          pGRRD->GRAT[0] = SDRAT[0];
          pGRRD->GRAT[1] = SDRAT[1];
          pGRRD->GRAT[2] = SDRAT[2];
          pGRRD->GRAT[3] = SDRAT[3];
          nonstd::unique_ptr<CJBig2_ArithDecoder> pArithDecoder(
              new CJBig2_ArithDecoder(pStream));
          BS = pGRRD->decode(pArithDecoder.get(), vector_as_array(grContext));
          if (!BS) {
            FX_Free(SBSYMS);
            goto failed;
          }
          pStream->alignByte();
          pStream->offset(2);
          if ((FX_DWORD)nVal != (pStream->getOffset() - nTmp)) {
            delete BS;
            FX_Free(SBSYMS);
            goto failed;
          }
          FX_Free(SBSYMS);
        }
        SDNEWSYMS[NSYMSDECODED] = BS;
      }
      if (SDREFAGG == 0) {
        SDNEWSYMWIDTHS[NSYMSDECODED] = SYMWIDTH;
      }
      NSYMSDECODED = NSYMSDECODED + 1;
    }
    if (SDREFAGG == 0) {
      if (pHuffmanDecoder->decodeAValue(SDHUFFBMSIZE, (int32_t*)&BMSIZE) != 0) {
        goto failed;
      }
      pStream->alignByte();
      if (BMSIZE == 0) {
        stride = (TOTWIDTH + 7) >> 3;
        if (pStream->getByteLeft() >= stride * HCHEIGHT) {
          BHC = new CJBig2_Image(TOTWIDTH, HCHEIGHT);
          for (I = 0; I < HCHEIGHT; I++) {
            JBIG2_memcpy(BHC->m_pData + I * BHC->m_nStride,
                         pStream->getPointer(), stride);
            pStream->offset(stride);
          }
        } else {
          goto failed;
        }
      } else {
        nonstd::unique_ptr<CJBig2_GRDProc> pGRD(new CJBig2_GRDProc());
        pGRD->MMR = 1;
        pGRD->GBW = TOTWIDTH;
        pGRD->GBH = HCHEIGHT;
        FXCODEC_STATUS status = pGRD->Start_decode_MMR(&BHC, pStream, nullptr);
        while (status == FXCODEC_STATUS_DECODE_TOBECONTINUE) {
          pGRD->Continue_decode(pPause);
        }
        pStream->alignByte();
      }
      nTmp = 0;
      if (!BHC) {
        continue;
      }
      for (I = HCFIRSTSYM; I < NSYMSDECODED; I++) {
        SDNEWSYMS[I] = BHC->subImage(nTmp, 0, SDNEWSYMWIDTHS[I], HCHEIGHT);
        nTmp += SDNEWSYMWIDTHS[I];
      }
      delete BHC;
      BHC = nullptr;
    }
  }
  EXINDEX = 0;
  CUREXFLAG = 0;
  pTable.reset(new CJBig2_HuffmanTable(
      HuffmanTable_B1, FX_ArraySize(HuffmanTable_B1), HuffmanTable_HTOOB_B1));
  EXFLAGS = FX_Alloc(FX_BOOL, SDNUMINSYMS + SDNUMNEWSYMS);
  while (EXINDEX < SDNUMINSYMS + SDNUMNEWSYMS) {
    if (pHuffmanDecoder->decodeAValue(pTable.get(), (int*)&EXRUNLENGTH) != 0) {
      FX_Free(EXFLAGS);
      goto failed;
    }
    if (EXINDEX + EXRUNLENGTH > SDNUMINSYMS + SDNUMNEWSYMS) {
      FX_Free(EXFLAGS);
      goto failed;
    }
    if (EXRUNLENGTH != 0) {
      for (I = EXINDEX; I < EXINDEX + EXRUNLENGTH; I++) {
        EXFLAGS[I] = CUREXFLAG;
      }
    }
    EXINDEX = EXINDEX + EXRUNLENGTH;
    CUREXFLAG = !CUREXFLAG;
  }
  I = J = 0;
  for (I = 0; I < SDNUMINSYMS + SDNUMNEWSYMS; I++) {
    if (EXFLAGS[I] && J < SDNUMEXSYMS) {
      if (I < SDNUMINSYMS) {
        pDict->AddImage(new CJBig2_Image(*SDINSYMS[I]));
      } else {
        pDict->AddImage(SDNEWSYMS[I - SDNUMINSYMS]);
      }
      ++J;
    } else if (!EXFLAGS[I] && I >= SDNUMINSYMS) {
      delete SDNEWSYMS[I - SDNUMINSYMS];
    }
  }
  FX_Free(EXFLAGS);
  FX_Free(SDNEWSYMS);
  if (SDREFAGG == 0) {
    FX_Free(SDNEWSYMWIDTHS);
  }
  return pDict.release();
failed:
  for (I = 0; I < NSYMSDECODED; I++) {
    delete SDNEWSYMS[I];
  }
  FX_Free(SDNEWSYMS);
  if (SDREFAGG == 0) {
    FX_Free(SDNEWSYMWIDTHS);
  }
  return nullptr;
}

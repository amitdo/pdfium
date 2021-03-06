// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef CORE_SRC_FXCODEC_JBIG2_JBIG2_SYMBOLDICT_H_
#define CORE_SRC_FXCODEC_JBIG2_JBIG2_SYMBOLDICT_H_

#include <vector>

#include "../../../../third_party/base/nonstd_unique_ptr.h"
#include "../../../include/fxcrt/fx_basic.h"
#include "JBig2_ArithDecoder.h"
#include "JBig2_List.h"

class CJBig2_Image;

class CJBig2_SymbolDict {
 public:
  CJBig2_SymbolDict();
  ~CJBig2_SymbolDict();

  nonstd::unique_ptr<CJBig2_SymbolDict> DeepCopy() const;

  // Takes ownership of |image|.
  void AddImage(CJBig2_Image* image) { m_SDEXSYMS.push_back(image); }

  size_t NumImages() const { return m_SDEXSYMS.size(); }
  CJBig2_Image* GetImage(size_t index) const { return m_SDEXSYMS.get(index); }

  const std::vector<JBig2ArithCtx>& GbContext() const { return m_gbContext; }
  const std::vector<JBig2ArithCtx>& GrContext() const { return m_grContext; }

  void SetGbContext(const std::vector<JBig2ArithCtx>& gbContext) {
    m_gbContext = gbContext;
  }
  void SetGrContext(const std::vector<JBig2ArithCtx>& grContext) {
    m_grContext = grContext;
  }

 private:
  std::vector<JBig2ArithCtx> m_gbContext;
  std::vector<JBig2ArithCtx> m_grContext;
  CJBig2_List<CJBig2_Image> m_SDEXSYMS;
};

#endif  // CORE_SRC_FXCODEC_JBIG2_JBIG2_SYMBOLDICT_H_

// Copyright 2014 PDFium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Original code copyright 2014 Foxit Software Inc. http://www.foxitsoftware.com

#ifndef FPDFSDK_SRC_JAVASCRIPT_DOCUMENT_H_
#define FPDFSDK_SRC_JAVASCRIPT_DOCUMENT_H_

#include "JS_Define.h"

class PrintParamsObj : public CJS_EmbedObj {
 public:
  PrintParamsObj(CJS_Object* pJSObject);
  ~PrintParamsObj() override {}

 public:
  FX_BOOL bUI;
  int nStart;
  int nEnd;
  FX_BOOL bSilent;
  FX_BOOL bShrinkToFit;
  FX_BOOL bPrintAsImage;
  FX_BOOL bReverse;
  FX_BOOL bAnnotations;
};

class CJS_PrintParamsObj : public CJS_Object {
 public:
  CJS_PrintParamsObj(v8::Local<v8::Object> pObject) : CJS_Object(pObject) {}
  ~CJS_PrintParamsObj() override {}

  DECLARE_JS_CLASS();
};

class Icon;
class Field;

struct IconElement {
  IconElement() : IconName(L""), NextIcon(NULL), IconStream(NULL) {}
  virtual ~IconElement() {}
  CFX_WideString IconName;
  IconElement* NextIcon;
  Icon* IconStream;
};

class IconTree {
 public:
  IconTree() : m_pHead(NULL), m_pEnd(NULL), m_iLength(0) {}

  virtual ~IconTree() {}

 public:
  void InsertIconElement(IconElement* pNewIcon);
  void DeleteIconTree();
  int GetLength();
  IconElement* operator[](int iIndex);

 private:
  IconElement* m_pHead;
  IconElement* m_pEnd;
  int m_iLength;
};

struct CJS_DelayData;
struct CJS_DelayAnnot;
struct CJS_AnnotObj;

class Document : public CJS_EmbedObj {
 public:
  Document(CJS_Object* pJSObject);
  ~Document() override;

 public:
  FX_BOOL ADBE(IJS_Context* cc, CJS_PropValue& vp, CFX_WideString& sError);
  FX_BOOL author(IJS_Context* cc, CJS_PropValue& vp, CFX_WideString& sError);
  FX_BOOL baseURL(IJS_Context* cc, CJS_PropValue& vp, CFX_WideString& sError);
  FX_BOOL bookmarkRoot(IJS_Context* cc,
                       CJS_PropValue& vp,
                       CFX_WideString& sError);
  FX_BOOL calculate(IJS_Context* cc, CJS_PropValue& vp, CFX_WideString& sError);
  FX_BOOL Collab(IJS_Context* cc, CJS_PropValue& vp, CFX_WideString& sError);
  FX_BOOL creationDate(IJS_Context* cc,
                       CJS_PropValue& vp,
                       CFX_WideString& sError);
  FX_BOOL creator(IJS_Context* cc, CJS_PropValue& vp, CFX_WideString& sError);
  FX_BOOL delay(IJS_Context* cc, CJS_PropValue& vp, CFX_WideString& sError);
  FX_BOOL dirty(IJS_Context* cc, CJS_PropValue& vp, CFX_WideString& sError);
  FX_BOOL documentFileName(IJS_Context* cc,
                           CJS_PropValue& vp,
                           CFX_WideString& sError);
  FX_BOOL external(IJS_Context* cc, CJS_PropValue& vp, CFX_WideString& sError);
  FX_BOOL filesize(IJS_Context* cc, CJS_PropValue& vp, CFX_WideString& sError);
  FX_BOOL icons(IJS_Context* cc, CJS_PropValue& vp, CFX_WideString& sError);
  FX_BOOL info(IJS_Context* cc, CJS_PropValue& vp, CFX_WideString& sError);
  FX_BOOL keywords(IJS_Context* cc, CJS_PropValue& vp, CFX_WideString& sError);
  FX_BOOL layout(IJS_Context* cc, CJS_PropValue& vp, CFX_WideString& sError);
  FX_BOOL media(IJS_Context* cc, CJS_PropValue& vp, CFX_WideString& sError);
  FX_BOOL modDate(IJS_Context* cc, CJS_PropValue& vp, CFX_WideString& sError);
  FX_BOOL mouseX(IJS_Context* cc, CJS_PropValue& vp, CFX_WideString& sError);
  FX_BOOL mouseY(IJS_Context* cc, CJS_PropValue& vp, CFX_WideString& sError);
  FX_BOOL numFields(IJS_Context* cc, CJS_PropValue& vp, CFX_WideString& sError);
  FX_BOOL numPages(IJS_Context* cc, CJS_PropValue& vp, CFX_WideString& sError);
  FX_BOOL pageNum(IJS_Context* cc, CJS_PropValue& vp, CFX_WideString& sError);
  FX_BOOL pageWindowRect(IJS_Context* cc,
                         CJS_PropValue& vp,
                         CFX_WideString& sError);
  FX_BOOL path(IJS_Context* cc, CJS_PropValue& vp, CFX_WideString& sError);
  FX_BOOL producer(IJS_Context* cc, CJS_PropValue& vp, CFX_WideString& sError);
  FX_BOOL subject(IJS_Context* cc, CJS_PropValue& vp, CFX_WideString& sError);
  FX_BOOL title(IJS_Context* cc, CJS_PropValue& vp, CFX_WideString& sError);
  FX_BOOL zoom(IJS_Context* cc, CJS_PropValue& vp, CFX_WideString& sError);
  FX_BOOL zoomType(IJS_Context* cc, CJS_PropValue& vp, CFX_WideString& sError);

  FX_BOOL addAnnot(IJS_Context* cc,
                   const CJS_Parameters& params,
                   CJS_Value& vRet,
                   CFX_WideString& sError);
  FX_BOOL addField(IJS_Context* cc,
                   const CJS_Parameters& params,
                   CJS_Value& vRet,
                   CFX_WideString& sError);
  FX_BOOL addLink(IJS_Context* cc,
                  const CJS_Parameters& params,
                  CJS_Value& vRet,
                  CFX_WideString& sError);
  FX_BOOL addIcon(IJS_Context* cc,
                  const CJS_Parameters& params,
                  CJS_Value& vRet,
                  CFX_WideString& sError);
  FX_BOOL calculateNow(IJS_Context* cc,
                       const CJS_Parameters& params,
                       CJS_Value& vRet,
                       CFX_WideString& sError);
  FX_BOOL closeDoc(IJS_Context* cc,
                   const CJS_Parameters& params,
                   CJS_Value& vRet,
                   CFX_WideString& sError);
  FX_BOOL createDataObject(IJS_Context* cc,
                           const CJS_Parameters& params,
                           CJS_Value& vRet,
                           CFX_WideString& sError);
  FX_BOOL deletePages(IJS_Context* cc,
                      const CJS_Parameters& params,
                      CJS_Value& vRet,
                      CFX_WideString& sError);
  FX_BOOL exportAsText(IJS_Context* cc,
                       const CJS_Parameters& params,
                       CJS_Value& vRet,
                       CFX_WideString& sError);
  FX_BOOL exportAsFDF(IJS_Context* cc,
                      const CJS_Parameters& params,
                      CJS_Value& vRet,
                      CFX_WideString& sError);
  FX_BOOL exportAsXFDF(IJS_Context* cc,
                       const CJS_Parameters& params,
                       CJS_Value& vRet,
                       CFX_WideString& sError);
  FX_BOOL extractPages(IJS_Context* cc,
                       const CJS_Parameters& params,
                       CJS_Value& vRet,
                       CFX_WideString& sError);
  FX_BOOL getAnnot(IJS_Context* cc,
                   const CJS_Parameters& params,
                   CJS_Value& vRet,
                   CFX_WideString& sError);
  FX_BOOL getAnnots(IJS_Context* cc,
                    const CJS_Parameters& params,
                    CJS_Value& vRet,
                    CFX_WideString& sError);
  FX_BOOL getAnnot3D(IJS_Context* cc,
                     const CJS_Parameters& params,
                     CJS_Value& vRet,
                     CFX_WideString& sError);
  FX_BOOL getAnnots3D(IJS_Context* cc,
                      const CJS_Parameters& params,
                      CJS_Value& vRet,
                      CFX_WideString& sError);
  FX_BOOL getField(IJS_Context* cc,
                   const CJS_Parameters& params,
                   CJS_Value& vRet,
                   CFX_WideString& sError);
  FX_BOOL getIcon(IJS_Context* cc,
                  const CJS_Parameters& params,
                  CJS_Value& vRet,
                  CFX_WideString& sError);
  FX_BOOL getLinks(IJS_Context* cc,
                   const CJS_Parameters& params,
                   CJS_Value& vRet,
                   CFX_WideString& sError);
  FX_BOOL getNthFieldName(IJS_Context* cc,
                          const CJS_Parameters& params,
                          CJS_Value& vRet,
                          CFX_WideString& sError);
  FX_BOOL getOCGs(IJS_Context* cc,
                  const CJS_Parameters& params,
                  CJS_Value& vRet,
                  CFX_WideString& sError);
  FX_BOOL getPageBox(IJS_Context* cc,
                     const CJS_Parameters& params,
                     CJS_Value& vRet,
                     CFX_WideString& sError);
  FX_BOOL getPageNthWord(IJS_Context* cc,
                         const CJS_Parameters& params,
                         CJS_Value& vRet,
                         CFX_WideString& sError);
  FX_BOOL getPageNthWordQuads(IJS_Context* cc,
                              const CJS_Parameters& params,
                              CJS_Value& vRet,
                              CFX_WideString& sError);
  FX_BOOL getPageNumWords(IJS_Context* cc,
                          const CJS_Parameters& params,
                          CJS_Value& vRet,
                          CFX_WideString& sError);
  FX_BOOL getPrintParams(IJS_Context* cc,
                         const CJS_Parameters& params,
                         CJS_Value& vRet,
                         CFX_WideString& sError);
  FX_BOOL getURL(IJS_Context* cc,
                 const CJS_Parameters& params,
                 CJS_Value& vRet,
                 CFX_WideString& sError);
  FX_BOOL importAnFDF(IJS_Context* cc,
                      const CJS_Parameters& params,
                      CJS_Value& vRet,
                      CFX_WideString& sError);
  FX_BOOL importAnXFDF(IJS_Context* cc,
                       const CJS_Parameters& params,
                       CJS_Value& vRet,
                       CFX_WideString& sError);
  FX_BOOL importTextData(IJS_Context* cc,
                         const CJS_Parameters& params,
                         CJS_Value& vRet,
                         CFX_WideString& sError);
  FX_BOOL insertPages(IJS_Context* cc,
                      const CJS_Parameters& params,
                      CJS_Value& vRet,
                      CFX_WideString& sError);
  FX_BOOL mailForm(IJS_Context* cc,
                   const CJS_Parameters& params,
                   CJS_Value& vRet,
                   CFX_WideString& sError);
  FX_BOOL print(IJS_Context* cc,
                const CJS_Parameters& params,
                CJS_Value& vRet,
                CFX_WideString& sError);
  FX_BOOL removeField(IJS_Context* cc,
                      const CJS_Parameters& params,
                      CJS_Value& vRet,
                      CFX_WideString& sError);
  FX_BOOL replacePages(IJS_Context* cc,
                       const CJS_Parameters& params,
                       CJS_Value& vRet,
                       CFX_WideString& sError);
  FX_BOOL resetForm(IJS_Context* cc,
                    const CJS_Parameters& params,
                    CJS_Value& vRet,
                    CFX_WideString& sError);
  FX_BOOL saveAs(IJS_Context* cc,
                 const CJS_Parameters& params,
                 CJS_Value& vRet,
                 CFX_WideString& sError);
  FX_BOOL submitForm(IJS_Context* cc,
                     const CJS_Parameters& params,
                     CJS_Value& vRet,
                     CFX_WideString& sError);
  FX_BOOL mailDoc(IJS_Context* cc,
                  const CJS_Parameters& params,
                  CJS_Value& vRet,
                  CFX_WideString& sError);
  FX_BOOL removeIcon(IJS_Context* cc,
                     const CJS_Parameters& params,
                     CJS_Value& vRet,
                     CFX_WideString& sError);

 public:
  void AttachDoc(CPDFSDK_Document* pDoc);
  CPDFSDK_Document* GetReaderDoc();
  static FX_BOOL ExtractFileName(CPDFSDK_Document* pDoc,
                                 CFX_ByteString& strFileName);
  static FX_BOOL ExtractFolderName(CPDFSDK_Document* pDoc,
                                   CFX_ByteString& strFolderName);
  void AddDelayData(CJS_DelayData* pData);
  void DoFieldDelay(const CFX_WideString& sFieldName, int nControlIndex);
  void AddDelayAnnotData(CJS_AnnotObj* pData);
  void DoAnnotDelay();
  void SetIsolate(v8::Isolate* isolate) { m_isolate = isolate; }
  CJS_Document* GetCJSDoc() const;

 private:
  CFX_WideString ReversalStr(CFX_WideString cbFrom);
  CFX_WideString CutString(CFX_WideString cbFrom);
  bool IsEnclosedInRect(CFX_FloatRect rect, CFX_FloatRect LinkRect);
  int CountWords(CPDF_TextObject* pTextObj);
  CFX_WideString GetObjWordStr(CPDF_TextObject* pTextObj, int nWordIndex);

  v8::Isolate* m_isolate;
  IconTree* m_pIconTree;
  CPDFSDK_Document* m_pDocument;
  CFX_WideString m_cwBaseURL;
  bool m_bDelay;
  CFX_ArrayTemplate<CJS_DelayData*> m_DelayData;
  CFX_ArrayTemplate<CJS_AnnotObj*> m_DelayAnnotData;
};

class CJS_Document : public CJS_Object {
 public:
  explicit CJS_Document(v8::Local<v8::Object> pObject) : CJS_Object(pObject) {}
  ~CJS_Document() override {}

  // CJS_Object
  void InitInstance(IJS_Runtime* pIRuntime) override;

  DECLARE_JS_CLASS();

  JS_STATIC_PROP(ADBE, Document);
  JS_STATIC_PROP(author, Document);
  JS_STATIC_PROP(baseURL, Document);
  JS_STATIC_PROP(bookmarkRoot, Document);
  JS_STATIC_PROP(calculate, Document);
  JS_STATIC_PROP(Collab, Document);
  JS_STATIC_PROP(creationDate, Document);
  JS_STATIC_PROP(creator, Document);
  JS_STATIC_PROP(delay, Document);
  JS_STATIC_PROP(dirty, Document);
  JS_STATIC_PROP(documentFileName, Document);
  JS_STATIC_PROP(external, Document);
  JS_STATIC_PROP(filesize, Document);
  JS_STATIC_PROP(icons, Document);
  JS_STATIC_PROP(info, Document);
  JS_STATIC_PROP(keywords, Document);
  JS_STATIC_PROP(layout, Document);
  JS_STATIC_PROP(media, Document);
  JS_STATIC_PROP(modDate, Document);
  JS_STATIC_PROP(mouseX, Document);
  JS_STATIC_PROP(mouseY, Document);
  JS_STATIC_PROP(numFields, Document);
  JS_STATIC_PROP(numPages, Document);
  JS_STATIC_PROP(pageNum, Document);
  JS_STATIC_PROP(pageWindowRect, Document);
  JS_STATIC_PROP(path, Document);
  JS_STATIC_PROP(producer, Document);
  JS_STATIC_PROP(subject, Document);
  JS_STATIC_PROP(title, Document);
  JS_STATIC_PROP(zoom, Document);
  JS_STATIC_PROP(zoomType, Document);

  JS_STATIC_METHOD(addAnnot, Document);
  JS_STATIC_METHOD(addField, Document);
  JS_STATIC_METHOD(addLink, Document);
  JS_STATIC_METHOD(addIcon, Document);
  JS_STATIC_METHOD(calculateNow, Document);
  JS_STATIC_METHOD(closeDoc, Document);
  JS_STATIC_METHOD(createDataObject, Document);
  JS_STATIC_METHOD(deletePages, Document);
  JS_STATIC_METHOD(exportAsText, Document);
  JS_STATIC_METHOD(exportAsFDF, Document);
  JS_STATIC_METHOD(exportAsXFDF, Document);
  JS_STATIC_METHOD(extractPages, Document);
  JS_STATIC_METHOD(getAnnot, Document);
  JS_STATIC_METHOD(getAnnots, Document);
  JS_STATIC_METHOD(getAnnot3D, Document);
  JS_STATIC_METHOD(getAnnots3D, Document);
  JS_STATIC_METHOD(getField, Document);
  JS_STATIC_METHOD(getIcon, Document);
  JS_STATIC_METHOD(getLinks, Document);
  JS_STATIC_METHOD(getNthFieldName, Document);
  JS_STATIC_METHOD(getOCGs, Document);
  JS_STATIC_METHOD(getPageBox, Document);
  JS_STATIC_METHOD(getPageNthWord, Document);
  JS_STATIC_METHOD(getPageNthWordQuads, Document);
  JS_STATIC_METHOD(getPageNumWords, Document);
  JS_STATIC_METHOD(getPrintParams, Document);
  JS_STATIC_METHOD(getURL, Document);
  JS_STATIC_METHOD(importAnFDF, Document);
  JS_STATIC_METHOD(importAnXFDF, Document);
  JS_STATIC_METHOD(importTextData, Document);
  JS_STATIC_METHOD(insertPages, Document);
  JS_STATIC_METHOD(mailForm, Document);
  JS_STATIC_METHOD(print, Document);
  JS_STATIC_METHOD(removeField, Document);
  JS_STATIC_METHOD(replacePages, Document);
  JS_STATIC_METHOD(removeIcon, Document);
  JS_STATIC_METHOD(resetForm, Document);
  JS_STATIC_METHOD(saveAs, Document);
  JS_STATIC_METHOD(submitForm, Document);
  JS_STATIC_METHOD(mailDoc, Document);
};

#endif  // FPDFSDK_SRC_JAVASCRIPT_DOCUMENT_H_

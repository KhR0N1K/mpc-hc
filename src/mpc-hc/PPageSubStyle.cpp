/*
 * (C) 2003-2006 Gabest
 * (C) 2006-2014 see Authors.txt
 *
 * This file is part of MPC-HC.
 *
 * MPC-HC is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * MPC-HC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "stdafx.h"
#include <math.h>
#include "mplayerc.h"
#include "MainFrm.h"
#include "PPageSubStyle.h"

// CPPageSubStyle dialog

#pragma warning(push)
#pragma warning(disable: 4351) // new behavior: elements of array 'array' will be default initialized
IMPLEMENT_DYNAMIC(CPPageSubStyle, CPPageBase)
CPPageSubStyle::CPPageSubStyle()
    : CPPageBase(CPPageSubStyle::IDD, CPPageSubStyle::IDD)
    , m_iCharset(0)
    , m_spacing(0)
    , m_angle(0)
    , m_scalex(0)
    , m_scaley(0)
    , m_borderstyle(0)
    , m_borderwidth(0)
    , m_shadowdepth(0)
    , m_screenalignment(0)
    , m_margin(0, 0, 0, 0)
    , m_linkalphasliders(FALSE)
    , m_iRelativeTo(0)
    , m_fUseDefaultStyle(true)
    , m_stss(AfxGetAppSettings().subtitlesDefStyle)
    , m_alpha()
{
}
#pragma warning(pop)

CPPageSubStyle::~CPPageSubStyle()
{
}

void CPPageSubStyle::InitStyle(CString title, const STSStyle& stss)
{
    m_pPSP->pszTitle = (m_title = title);
    m_psp.dwFlags |= PSP_USETITLE;

    m_stss = stss;
    m_fUseDefaultStyle = false;
}

void CPPageSubStyle::AskColor(int i)
{
    CColorDialog dlg(m_stss.colors[i]);
    dlg.m_cc.Flags |= CC_FULLOPEN;
    if (dlg.DoModal() == IDOK) {
        m_stss.colors[i] = dlg.m_cc.rgbResult;
        m_color[i].SetColor(dlg.m_cc.rgbResult);
        SetModified();
    }
}

void CPPageSubStyle::DoDataExchange(CDataExchange* pDX)
{
    CPPageBase::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_BUTTON1, m_font);
    DDX_CBIndex(pDX, IDC_COMBO1, m_iCharset);
    DDX_Control(pDX, IDC_COMBO1, m_charset);
    DDX_Text(pDX, IDC_EDIT3, m_spacing);
    DDX_Control(pDX, IDC_SPIN3, m_spacingspin);
    DDX_Text(pDX, IDC_EDIT4, m_angle);
    DDX_Control(pDX, IDC_SPIN10, m_anglespin);
    DDX_Text(pDX, IDC_EDIT5, m_scalex);
    DDX_Control(pDX, IDC_SPIN4, m_scalexspin);
    DDX_Text(pDX, IDC_EDIT6, m_scaley);
    DDX_Control(pDX, IDC_SPIN5, m_scaleyspin);
    DDX_Radio(pDX, IDC_RADIO1, m_borderstyle);
    DDX_Text(pDX, IDC_EDIT1, m_borderwidth);
    DDX_Control(pDX, IDC_SPIN1, m_borderwidthspin);
    DDX_Text(pDX, IDC_EDIT2, m_shadowdepth);
    DDX_Control(pDX, IDC_SPIN2, m_shadowdepthspin);
    DDX_Radio(pDX, IDC_RADIO3, m_screenalignment);
    DDX_Text(pDX, IDC_EDIT7, m_margin.left);
    DDX_Control(pDX, IDC_SPIN6, m_marginleftspin);
    DDX_Text(pDX, IDC_EDIT8, m_margin.right);
    DDX_Control(pDX, IDC_SPIN7, m_marginrightspin);
    DDX_Text(pDX, IDC_EDIT9, m_margin.top);
    DDX_Control(pDX, IDC_SPIN8, m_margintopspin);
    DDX_Text(pDX, IDC_EDIT10, m_margin.bottom);
    DDX_Control(pDX, IDC_SPIN9, m_marginbottomspin);
    DDX_Control(pDX, IDC_COLORPRI, m_color[0]);
    DDX_Control(pDX, IDC_COLORSEC, m_color[1]);
    DDX_Control(pDX, IDC_COLOROUTL, m_color[2]);
    DDX_Control(pDX, IDC_COLORSHAD, m_color[3]);
    DDX_Slider(pDX, IDC_SLIDER1, m_alpha[0]);
    DDX_Slider(pDX, IDC_SLIDER2, m_alpha[1]);
    DDX_Slider(pDX, IDC_SLIDER3, m_alpha[2]);
    DDX_Slider(pDX, IDC_SLIDER4, m_alpha[3]);
    DDX_Control(pDX, IDC_SLIDER1, m_alphasliders[0]);
    DDX_Control(pDX, IDC_SLIDER2, m_alphasliders[1]);
    DDX_Control(pDX, IDC_SLIDER3, m_alphasliders[2]);
    DDX_Control(pDX, IDC_SLIDER4, m_alphasliders[3]);
    DDX_Check(pDX, IDC_CHECK1, m_linkalphasliders);
    DDX_Check(pDX, IDC_CHECK_RELATIVETO, m_iRelativeTo);
}


BEGIN_MESSAGE_MAP(CPPageSubStyle, CPPageBase)
    ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
    ON_BN_CLICKED(IDC_COLORPRI, OnStnClickedColorpri)
    ON_BN_CLICKED(IDC_COLORSEC, OnStnClickedColorsec)
    ON_BN_CLICKED(IDC_COLOROUTL, OnStnClickedColoroutl)
    ON_BN_CLICKED(IDC_COLORSHAD, OnStnClickedColorshad)
    ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
    ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CPPageSubStyle message handlers

BOOL CPPageSubStyle::OnInitDialog()
{
    __super::OnInitDialog();

    SetHandCursor(m_hWnd, IDC_COMBO1);

    m_font.SetWindowText(m_stss.fontName);
    m_iCharset = -1;
    for (int i = 0; i < CharSetLen; i++) {
        CString str;
        str.Format(_T("%s (%u)"), CharSetNames[i], CharSetList[i]);
        m_charset.AddString(str);
        m_charset.SetItemData(i, CharSetList[i]);
        if (m_stss.charSet == CharSetList[i]) {
            m_iCharset = i;
        }
    }

    // TODO: allow floats in these edit boxes
    m_spacing = (int)m_stss.fontSpacing;
    m_spacingspin.SetRange32(-10000, 10000);
    while (m_stss.fontAngleZ < 0) {
        m_stss.fontAngleZ += 360;
    }
    m_angle = (int)fmod(m_stss.fontAngleZ, 360);
    m_anglespin.SetRange32(0, 359);
    m_scalex = (int)m_stss.fontScaleX;
    m_scalexspin.SetRange32(-10000, 10000);
    m_scaley = (int)m_stss.fontScaleY;
    m_scaleyspin.SetRange32(-10000, 10000);

    m_borderstyle = m_stss.borderStyle;
    m_borderwidth = (int)min(m_stss.outlineWidthX, m_stss.outlineWidthY);
    m_borderwidthspin.SetRange32(0, 10000);
    m_shadowdepth = (int)min(m_stss.shadowDepthX, m_stss.shadowDepthY);
    m_shadowdepthspin.SetRange32(0, 10000);

    m_screenalignment = m_stss.scrAlignment - 1;
    m_margin = m_stss.marginRect;
    m_marginleftspin.SetRange32(-10000, 10000);
    m_marginrightspin.SetRange32(-10000, 10000);
    m_margintopspin.SetRange32(-10000, 10000);
    m_marginbottomspin.SetRange32(-10000, 10000);
    m_iRelativeTo = m_stss.relativeTo;

    for (int i = 0; i < 4; i++) {
        m_color[i].SetColor(m_stss.colors[i]);
        m_alpha[i] = 255 - m_stss.alpha[i];
        m_alphasliders[i].SetRange(0, 255);
    }

    m_linkalphasliders = FALSE;

    UpdateData(FALSE);

    CreateToolTip();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPPageSubStyle::OnApply()
{
    UpdateData();

    if (m_iCharset >= 0) {
        m_stss.charSet = (int)m_charset.GetItemData(m_iCharset);
    }
    m_stss.fontSpacing = m_spacing;
    m_stss.fontAngleZ = m_angle;
    m_stss.fontScaleX = m_scalex;
    m_stss.fontScaleY = m_scaley;

    m_stss.borderStyle = m_borderstyle;
    m_stss.outlineWidthX = m_stss.outlineWidthY = m_borderwidth;
    m_stss.shadowDepthX = m_stss.shadowDepthY = m_shadowdepth;

    m_stss.scrAlignment = m_screenalignment + 1;
    m_stss.marginRect = m_margin;
    m_stss.relativeTo = m_iRelativeTo;

    for (int i = 0; i < 4; i++) {
        m_stss.alpha[i] = 255 - m_alpha[i];
    }

    if (m_fUseDefaultStyle) {
        STSStyle& stss = AfxGetAppSettings().subtitlesDefStyle;
        if (stss != m_stss) {
            stss = m_stss;
            if (CMainFrame* pFrame = dynamic_cast<CMainFrame*>(AfxGetMainWnd())) {
                pFrame->SetSubtitle(0, true, false, true);
            }
        }
    }

    return __super::OnApply();
}

void CPPageSubStyle::OnBnClickedButton1()
{
    UpdateData();

    LOGFONT lf;
    lf <<= m_stss;
    if (m_iCharset >= 0) {
        lf.lfCharSet = (BYTE)m_charset.GetItemData(m_iCharset);
    }

    CFontDialog dlg(&lf, CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT | CF_FORCEFONTEXIST | CF_SCALABLEONLY);
    if (dlg.DoModal() == IDOK) {
        CString str(lf.lfFaceName);
        if (str.GetLength() > 16) {
            str = str.Left(14) + _T("...");
        }
        m_font.SetWindowText(str);

        for (int i = 0, j = m_charset.GetCount(); i < j; i++) {
            if (m_charset.GetItemData(i) == lf.lfCharSet) {
                m_charset.SetCurSel(i);
                break;
            }
        }

        m_stss = lf;

        SetModified();
    }
}

void CPPageSubStyle::OnStnClickedColorpri()
{
    AskColor(0);
}

void CPPageSubStyle::OnStnClickedColorsec()
{
    AskColor(1);
}

void CPPageSubStyle::OnStnClickedColoroutl()
{
    AskColor(2);
}

void CPPageSubStyle::OnStnClickedColorshad()
{
    AskColor(3);
}

void CPPageSubStyle::OnBnClickedCheck1()
{
    UpdateData();

    int avg = 0;
    for (int i = 0; i < 4; i++) {
        avg += m_alphasliders[i].GetPos();
    }
    avg /= 4;
    for (int i = 0; i < 4; i++) {
        m_alphasliders[i].SetPos(avg);
    }

    SetModified();
}

void CPPageSubStyle::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    if (m_linkalphasliders && pScrollBar) {
        int pos = ((CSliderCtrl*)pScrollBar)->GetPos();
        for (int i = 0; i < 4; i++) {
            m_alphasliders[i].SetPos(pos);
        }
    }

    SetModified();

    __super::OnHScroll(nSBCode, nPos, pScrollBar);
}

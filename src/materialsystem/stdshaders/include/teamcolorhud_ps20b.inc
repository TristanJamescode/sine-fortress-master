// ALL SKIP STATEMENTS THAT AFFECT THIS SHADER!!!
// ($DISTANCEALPHAMODE >= 8 && ($DETAILTEXTUREMODE == 0 || $ALPHATEXTURE == 1))
// ($DISTANCEALPHAMODE != 0 && $WRITEWATERFOGTODESTALPHA == 1)
// ($DISTANCEALPHAMODE != 0 && $WRITE_DEPTH_TO_DESTALPHA == 1)
// defined $HDRTYPE && defined $HDRENABLED && !$HDRTYPE && $HDRENABLED
// defined $PIXELFOGTYPE && defined $WRITEWATERFOGTODESTALPHA && ( $PIXELFOGTYPE != 1 ) && $WRITEWATERFOGTODESTALPHA
// defined $LIGHTING_PREVIEW && defined $HDRTYPE && $LIGHTING_PREVIEW && $HDRTYPE != 0
// defined $LIGHTING_PREVIEW && defined $FASTPATHENVMAPTINT && $LIGHTING_PREVIEW && $FASTPATHENVMAPTINT
// defined $LIGHTING_PREVIEW && defined $FASTPATHENVMAPCONTRAST && $LIGHTING_PREVIEW && $FASTPATHENVMAPCONTRAST
// defined $LIGHTING_PREVIEW && defined $FASTPATH && $LIGHTING_PREVIEW && $FASTPATH
// ($FLASHLIGHT || $FLASHLIGHTSHADOWS) && $LIGHTING_PREVIEW

#pragma once
#include "shaderlib/cshader.h"
class teamcolorhud_ps20b_Static_Index
{
	unsigned int m_nDISTANCEALPHAMODE : 4;
	unsigned int m_nDETAILTEXTUREMODE : 4;
	unsigned int m_nBLENDTINTBYXALPHA : 2;
	unsigned int m_nVERTEXRGB : 2;
	unsigned int m_nVERTEXA : 2;
	unsigned int m_nALPHATEXTURE : 2;
#ifdef _DEBUG
	bool m_bDISTANCEALPHAMODE : 1;
	bool m_bDETAILTEXTUREMODE : 1;
	bool m_bBLENDTINTBYXALPHA : 1;
	bool m_bVERTEXRGB : 1;
	bool m_bVERTEXA : 1;
	bool m_bALPHATEXTURE : 1;
#endif	// _DEBUG
public:
	void SetDISTANCEALPHAMODE( int i )
	{
		Assert( i >= 0 && i <= 14 );
		m_nDISTANCEALPHAMODE = i;
#ifdef _DEBUG
		m_bDISTANCEALPHAMODE = true;
#endif	// _DEBUG
	}

	void SetDETAILTEXTUREMODE( int i )
	{
		Assert( i >= 0 && i <= 10 );
		m_nDETAILTEXTUREMODE = i;
#ifdef _DEBUG
		m_bDETAILTEXTUREMODE = true;
#endif	// _DEBUG
	}

	void SetBLENDTINTBYXALPHA( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nBLENDTINTBYXALPHA = i;
#ifdef _DEBUG
		m_bBLENDTINTBYXALPHA = true;
#endif	// _DEBUG
	}

	void SetVERTEXRGB( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nVERTEXRGB = i;
#ifdef _DEBUG
		m_bVERTEXRGB = true;
#endif	// _DEBUG
	}

	void SetVERTEXA( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nVERTEXA = i;
#ifdef _DEBUG
		m_bVERTEXA = true;
#endif	// _DEBUG
	}

	void SetALPHATEXTURE( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nALPHATEXTURE = i;
#ifdef _DEBUG
		m_bALPHATEXTURE = true;
#endif	// _DEBUG
	}

	teamcolorhud_ps20b_Static_Index(  )
	{
		m_nDISTANCEALPHAMODE = 0;
		m_nDETAILTEXTUREMODE = 0;
		m_nBLENDTINTBYXALPHA = 0;
		m_nVERTEXRGB = 0;
		m_nVERTEXA = 0;
		m_nALPHATEXTURE = 0;
#ifdef _DEBUG
		m_bDISTANCEALPHAMODE = false;
		m_bDETAILTEXTUREMODE = false;
		m_bBLENDTINTBYXALPHA = false;
		m_bVERTEXRGB = false;
		m_bVERTEXA = false;
		m_bALPHATEXTURE = false;
#endif	// _DEBUG
	}

	int GetIndex() const
	{
		Assert( m_bDISTANCEALPHAMODE && m_bDETAILTEXTUREMODE && m_bBLENDTINTBYXALPHA && m_bVERTEXRGB && m_bVERTEXA && m_bALPHATEXTURE );
		AssertMsg( !( ( m_nDISTANCEALPHAMODE >= 8 ) && ( ( m_nDETAILTEXTUREMODE == 0 ) || ( m_nALPHATEXTURE == 1 ) ) ), "Invalid combo combination ( ( DISTANCEALPHAMODE >= 8 ) && ( ( DETAILTEXTUREMODE == 0 ) || ( ALPHATEXTURE == 1 ) ) )" );
		return ( 1 * m_nDISTANCEALPHAMODE ) + ( 15 * m_nDETAILTEXTUREMODE ) + ( 165 * m_nBLENDTINTBYXALPHA ) + ( 330 * m_nVERTEXRGB ) + ( 660 * m_nVERTEXA ) + ( 1320 * m_nALPHATEXTURE ) + 0;
	}
};

#define shaderStaticTest_teamcolorhud_ps20b psh_forgot_to_set_static_DISTANCEALPHAMODE + psh_forgot_to_set_static_DETAILTEXTUREMODE + psh_forgot_to_set_static_BLENDTINTBYXALPHA + psh_forgot_to_set_static_VERTEXRGB + psh_forgot_to_set_static_VERTEXA + psh_forgot_to_set_static_ALPHATEXTURE


class teamcolorhud_ps20b_Dynamic_Index
{
public:
	teamcolorhud_ps20b_Dynamic_Index(  )
	{
	}

	int GetIndex() const
	{
		return 0;
	}
};

#define shaderDynamicTest_teamcolorhud_ps20b 1


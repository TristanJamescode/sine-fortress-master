// ALL SKIP STATEMENTS THAT AFFECT THIS SHADER!!!
// (!$VERTEXCOLOR && $DONT_GAMMA_CONVERT_VERTEX_COLOR)
// defined $LIGHTING_PREVIEW && defined $FASTPATH && $LIGHTING_PREVIEW && $FASTPATH

#pragma once
#include "shaderlib/cshader.h"
class teamcolorhud_vs20_Static_Index
{
	unsigned int m_nDETAILTEXTURE : 2;
	unsigned int m_nVERTEXCOLOR : 2;
	unsigned int m_nDONT_GAMMA_CONVERT_VERTEX_COLOR : 2;
#ifdef _DEBUG
	bool m_bDETAILTEXTURE : 1;
	bool m_bVERTEXCOLOR : 1;
	bool m_bDONT_GAMMA_CONVERT_VERTEX_COLOR : 1;
#endif	// _DEBUG
public:
	void SetDETAILTEXTURE( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nDETAILTEXTURE = i;
#ifdef _DEBUG
		m_bDETAILTEXTURE = true;
#endif	// _DEBUG
	}

	void SetVERTEXCOLOR( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nVERTEXCOLOR = i;
#ifdef _DEBUG
		m_bVERTEXCOLOR = true;
#endif	// _DEBUG
	}

	void SetDONT_GAMMA_CONVERT_VERTEX_COLOR( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nDONT_GAMMA_CONVERT_VERTEX_COLOR = i;
#ifdef _DEBUG
		m_bDONT_GAMMA_CONVERT_VERTEX_COLOR = true;
#endif	// _DEBUG
	}

	teamcolorhud_vs20_Static_Index(  )
	{
		m_nDETAILTEXTURE = 0;
		m_nVERTEXCOLOR = 0;
		m_nDONT_GAMMA_CONVERT_VERTEX_COLOR = 0;
#ifdef _DEBUG
		m_bDETAILTEXTURE = false;
		m_bVERTEXCOLOR = false;
		m_bDONT_GAMMA_CONVERT_VERTEX_COLOR = false;
#endif	// _DEBUG
	}

	int GetIndex() const
	{
		Assert( m_bDETAILTEXTURE && m_bVERTEXCOLOR && m_bDONT_GAMMA_CONVERT_VERTEX_COLOR );
		AssertMsg( !( !m_nVERTEXCOLOR && m_nDONT_GAMMA_CONVERT_VERTEX_COLOR ), "Invalid combo combination ( !VERTEXCOLOR && DONT_GAMMA_CONVERT_VERTEX_COLOR )" );
		return ( 4 * m_nDETAILTEXTURE ) + ( 8 * m_nVERTEXCOLOR ) + ( 16 * m_nDONT_GAMMA_CONVERT_VERTEX_COLOR ) + 0;
	}
};

#define shaderStaticTest_teamcolorhud_vs20 vsh_forgot_to_set_static_DETAILTEXTURE + vsh_forgot_to_set_static_VERTEXCOLOR + vsh_forgot_to_set_static_DONT_GAMMA_CONVERT_VERTEX_COLOR


class teamcolorhud_vs20_Dynamic_Index
{
	unsigned int m_nCOMPRESSED_VERTS : 2;
	unsigned int m_nSKINNING : 2;
#ifdef _DEBUG
	bool m_bCOMPRESSED_VERTS : 1;
	bool m_bSKINNING : 1;
#endif	// _DEBUG
public:
	void SetCOMPRESSED_VERTS( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nCOMPRESSED_VERTS = i;
#ifdef _DEBUG
		m_bCOMPRESSED_VERTS = true;
#endif	// _DEBUG
	}

	void SetSKINNING( int i )
	{
		Assert( i >= 0 && i <= 1 );
		m_nSKINNING = i;
#ifdef _DEBUG
		m_bSKINNING = true;
#endif	// _DEBUG
	}

	teamcolorhud_vs20_Dynamic_Index(  )
	{
		m_nCOMPRESSED_VERTS = 0;
		m_nSKINNING = 0;
#ifdef _DEBUG
		m_bCOMPRESSED_VERTS = false;
		m_bSKINNING = false;
#endif	// _DEBUG
	}

	int GetIndex() const
	{
		Assert( m_bCOMPRESSED_VERTS && m_bSKINNING );
		return ( 1 * m_nCOMPRESSED_VERTS ) + ( 2 * m_nSKINNING ) + 0;
	}
};

#define shaderDynamicTest_teamcolorhud_vs20 vsh_forgot_to_set_dynamic_COMPRESSED_VERTS + vsh_forgot_to_set_dynamic_SKINNING


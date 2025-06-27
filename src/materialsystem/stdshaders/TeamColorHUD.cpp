//===========================================================================//
//
// Purpose: Distance Alpha HUD Shader for Team Specific Colors
//
//===========================================================================//

#include "BaseVSShader.h"
#include "Color.h" // Required for ConColorMsg()

#include "teamcolorhud_vs20.inc"
#include "teamcolorhud_ps20b.inc"

// Such a classic..
// "memdbgon must be the last include file in a .cpp file!!!"
#include "tier0/memdbgon.h"

BEGIN_VS_SHADER(TeamColorHUD, "Unlit Shader for HUD, with Distance Alpha Support")

BEGIN_SHADER_PARAMS

// Detail Texture Support
// ========================= //
SHADER_PARAM(DETAIL, SHADER_PARAM_TYPE_TEXTURE, "", "")
SHADER_PARAM(DETAILTINT, SHADER_PARAM_TYPE_COLOR, "", "")
SHADER_PARAM(DETAILBLENDFACTOR, SHADER_PARAM_TYPE_FLOAT, "", "")
SHADER_PARAM(DETAILSCALE, SHADER_PARAM_TYPE_VEC2, "" ,"")
SHADER_PARAM(DETAILBLENDMODE, SHADER_PARAM_TYPE_INTEGER, "" ,"")
SHADER_PARAM(DETAILTEXTURETRANSFORM, SHADER_PARAM_TYPE_MATRIX, "", "")
SHADER_PARAM(DETAILFRAME, SHADER_PARAM_TYPE_INTEGER, "", "")

// Distance Alpha stuff
// ========================= //
SHADER_PARAM(DISTANCEALPHA, SHADER_PARAM_TYPE_BOOL, "", "")
SHADER_PARAM(DISTANCEALPHAFROMDETAIL, SHADER_PARAM_TYPE_BOOL, "", "")
SHADER_PARAM(SOFTEDGES, SHADER_PARAM_TYPE_BOOL, "", "")
SHADER_PARAM(SCALEEDGESOFTNESSBASEDONSCREENRES, SHADER_PARAM_TYPE_BOOL, "", "")
SHADER_PARAM(EDGESOFTNESSSTART, SHADER_PARAM_TYPE_FLOAT, "", "")
SHADER_PARAM(EDGESOFTNESSEND, SHADER_PARAM_TYPE_FLOAT, "", "")

SHADER_PARAM(GLOW, SHADER_PARAM_TYPE_BOOL,	 "", "")
SHADER_PARAM(GLOWCOLOR, SHADER_PARAM_TYPE_COLOR, "", "")
SHADER_PARAM(GLOWALPHA, SHADER_PARAM_TYPE_FLOAT, "", "")
SHADER_PARAM(GLOWSTART, SHADER_PARAM_TYPE_FLOAT, "", "")
SHADER_PARAM(GLOWEND, SHADER_PARAM_TYPE_FLOAT, "", "")
SHADER_PARAM(GLOWX, SHADER_PARAM_TYPE_FLOAT, "", "")
SHADER_PARAM(GLOWY, SHADER_PARAM_TYPE_FLOAT, "", "")

SHADER_PARAM(OUTLINE, SHADER_PARAM_TYPE_BOOL,	 "", "")
SHADER_PARAM(OUTLINECOLOR, SHADER_PARAM_TYPE_COLOR, "", "")
SHADER_PARAM(OUTLINEALPHA, SHADER_PARAM_TYPE_FLOAT, "", "")
SHADER_PARAM(OUTLINESTART0, SHADER_PARAM_TYPE_FLOAT, "", "")
SHADER_PARAM(OUTLINESTART1, SHADER_PARAM_TYPE_FLOAT, "", "")
SHADER_PARAM(OUTLINEEND0, SHADER_PARAM_TYPE_FLOAT, "", "")
SHADER_PARAM(OUTLINEEND1, SHADER_PARAM_TYPE_FLOAT, "", "")
SHADER_PARAM(SCALEOUTLINESOFTNESSBASEDONSCREENRES, SHADER_PARAM_TYPE_BOOL,	 "", "")

// Some Utility Things
// ========================= //
SHADER_PARAM(GAMMACOLORREAD, SHADER_PARAM_TYPE_INTEGER, "", "")
SHADER_PARAM(LINEARWRITE, SHADER_PARAM_TYPE_INTEGER, "", "")
SHADER_PARAM(ALPHATESTREFERENCE, SHADER_PARAM_TYPE_FLOAT, "", "")

// Main Features here!
SHADER_PARAM(BLENDTINTBYBASEALPHA, SHADER_PARAM_TYPE_BOOL, "", "")
SHADER_PARAM(BLENDTINTCOLOROVERBASE, SHADER_PARAM_TYPE_FLOAT, "", "")

// For feeding a dedicated AlphaTexture
// When using $BlendTintByBaseAlpha and $Detail, uses this for DistanceAlpha!
SHADER_PARAM(ALPHATEXTURE, SHADER_PARAM_TYPE_TEXTURE, "", "")
SHADER_PARAM(ALPHATEXTUREFRAME, SHADER_PARAM_TYPE_INTEGER, "", "")
END_SHADER_PARAMS

	SHADER_INIT_PARAMS()
	{
		// Unlit, so can't do SelfIllum
		CLEAR_FLAGS(MATERIAL_VAR_SELFILLUM);

		// Uh, Sure!
		SET_FLAGS2(MATERIAL_VAR2_SUPPORTS_HW_SKINNING);

		// No mat_fullbright for Decals, and whatever else this actually does BTS
		if (IS_FLAG_SET(MATERIAL_VAR_DECAL))
		{
			SET_FLAGS(MATERIAL_VAR_NO_DEBUG_OVERRIDE);
		}

		// Default Values
		// This looks much nicer on LUX
		// ========================= //

		// Detail
		if (!IsDefined(DETAILBLENDFACTOR))
			params[DETAILBLENDFACTOR]->SetFloatValue(1.0f);

		if (!IsDefined(DETAILSCALE))
			params[DETAILSCALE]->SetFloatValue(4.0f); // Stock-Consistency ( 1.0f would be better )

		// Distance Alpha
		if (!IsDefined(EDGESOFTNESSSTART))
			params[EDGESOFTNESSSTART]->SetFloatValue(0.5f);

		if (!IsDefined(EDGESOFTNESSEND))
			params[EDGESOFTNESSEND]->SetFloatValue(0.5f);

		if (!IsDefined(GLOWALPHA))
			params[GLOWALPHA]->SetFloatValue(1.0f);

		if (!IsDefined(OUTLINEALPHA))
			params[OUTLINEALPHA]->SetFloatValue(1.0f);

		// Warnings!
		// ========================= //
		if (IsDefined(DETAILBLENDMODE))
		{
			int nDetailBlendMode = params[DETAILBLENDMODE]->GetIntValue();
			if (nDetailBlendMode == 10 || nDetailBlendMode == 11)
			{
				ConColorMsg(Color(255, 191, 0, 255), pMaterialName);
				ConColorMsg(Color(252, 83, 83, 255), " uses an unsupported DetailBlendMode! ( 10 or 11 )\n");
			}

			// No Post-Lighting so can't do this
			if (nDetailBlendMode == 5 || nDetailBlendMode == 6)
			{
				ConColorMsg(Color(255, 191, 0, 255), pMaterialName);
				ConColorMsg(Color(252, 83, 83, 255), " uses an unsupported DetailBlendMode! ( 5 or 6 )\n");
			}
		}
	}

	// Only need to load Base and Detail
	SHADER_INIT
	{
		if(IsDefined(BASETEXTURE))
			LoadTexture(BASETEXTURE, params[GAMMACOLORREAD]->GetIntValue() != 0 ? 0 : TEXTUREFLAGS_SRGB);

		if (IsDefined(DETAIL))
		{
			int nDetailBlendMode = params[DETAILBLENDMODE]->GetIntValue();
			if ( nDetailBlendMode == 0 ) //Mod2X
				LoadTexture(DETAIL);
			else
				LoadTexture(DETAIL, TEXTUREFLAGS_SRGB);
		}

		if (IsDefined(ALPHATEXTURE))
			LoadTexture(ALPHATEXTURE);
	}
	
	SHADER_FALLBACK
	{
		return 0;
	}

	SHADER_DRAW
	{
		// Never Draw a Flashlight on this Shader
		// Attempting to do so could potentially crash the game from my experience
		if (IsSnapshotting() && IS_FLAG2_SET(MATERIAL_VAR2_USE_FLASHLIGHT))
		{
			Draw(false);
			return;
		}
		else if (pShaderAPI && pShaderAPI->InFlashlightMode())
		{
			Draw(false);
			return;
		}
	
		// Flags.. Just this one
		bool bAlphaTest = IS_FLAG_SET(MATERIAL_VAR_ALPHATEST);

		// Textures
		bool bHasBaseTexture = IsDefined(BASETEXTURE) && params[BASETEXTURE]->IsTexture();
		bool bHasDetailTexture = IsDefined(DETAIL) && params[DETAIL]->IsTexture();
		bool bHasAlphaTexture = IsDefined(ALPHATEXTURE) && params[ALPHATEXTURE]->IsTexture();
		bool bHasDistanceAlpha = params[DISTANCEALPHA]->GetIntValue() != 0;

		// Need this for Static Combo, sRGBRead and BlendType
		int nDetailBlendMode = params[DETAILBLENDMODE]->GetIntValue();

		// Determine whether or not we are using alpha
		// If we can write to the Dest Alpha
		// & What Blending we are supposed to use
		BlendType_t nBlendType;

		int nDetailTranslucencyTexture = -1;
		if (bHasDetailTexture)
		{
			// These Blendmodes modify the Output Alpha
			// So if they are used, Alpha Effects are possible,
			// even when BaseAlpha is used by something else
			if ((nDetailBlendMode == 3) || (nDetailBlendMode == 8) || (nDetailBlendMode == 9))
				nDetailTranslucencyTexture = DETAIL;
		}

		// if base alpha is used for tinting, ignore the base texture for computing translucency
		nBlendType = EvaluateBlendRequirements(BASETEXTURE, true, nDetailTranslucencyTexture);

		// If we have a $AlphaTexture, Translucent can be used ( BT_BLEND )
		if (bHasAlphaTexture && IS_FLAG_SET(MATERIAL_VAR_TRANSLUCENT) && !IS_FLAG_SET(MATERIAL_VAR_ALPHATEST))
			nBlendType = BT_BLEND;

		bool bIsFullyOpaque = (nBlendType != BT_BLENDADD) && (nBlendType != BT_BLEND) && !bAlphaTest;

		//==========================================================================//
		// Snapshot State
		//==========================================================================//
		SHADOW_STATE
		{
			// Common for HUD and UI Elements, so must support this
			bool bHasVertexRGB = IS_FLAG_SET(MATERIAL_VAR_VERTEXCOLOR);
			bool bHasVertexA = IS_FLAG_SET(MATERIAL_VAR_VERTEXALPHA);
			bool bBlendTintByBaseAlpha = params[BLENDTINTBYBASEALPHA]->GetIntValue() != 0;

			//==========================================================================//
			// General Rendering Setup Shenanigans
			//==========================================================================//
			
			// This handles : $IgnoreZ, $Decal, $Nocull, $Znearer, $Wireframe, $AllowAlphaToCoverage
			// Source : OrangeBox/ASW Code which have BaseShader.cpp
			SetInitialShadowState();

			// We need to account for the Whacky Blending Modes the Stock Shader Reference ( VLG ) does
			SetBlendingShadowState(nBlendType);

			// This is probably never gonna be true
			pShaderShadow->EnableAlphaWrites(bIsFullyOpaque);

			// Weird name, what it actually means : We output linear values when true
			bool bSRGBWrite = (params[LINEARWRITE]->GetIntValue() != 0) ? false : true;
			pShaderShadow->EnableSRGBWrite(bSRGBWrite);

			if (bAlphaTest)
			{
				pShaderShadow->EnableAlphaTest(true); // bAlphatest
				float f1AlphaTestReference = params[ALPHATESTREFERENCE]->GetFloatValue();
				if (f1AlphaTestReference > 0.0f) // 0 is default.
				{
					pShaderShadow->AlphaFunc(SHADER_ALPHAFUNC_GEQUAL, f1AlphaTestReference);
				}
			}

			//==========================================================================//
			// VertexFormat
			//==========================================================================//
			unsigned int nFlags = VERTEX_POSITION | VERTEX_FORMAT_COMPRESSED;

			if (bHasVertexRGB || bHasVertexA)
				nFlags |= VERTEX_COLOR;

			// Always.
			int nTexCoords = 1;

			// This would be 4 for bHasFlashlight or bHasBumpMap
			// When VertexCompression is NONE, it sends Tangent + BinormalSign (in .w)
			// To the TANGENT Vertex Stream, which just happens to be called vUserData!
			int nUserDataSize = 0;

			// Set that Format!
			pShaderShadow->VertexShaderVertexFormat(nFlags, nTexCoords, 0, nUserDataSize);

			//==========================================================================//
			// Enable Samplers
			//==========================================================================//

			// BaseTexture ( s0 ), not always sRGB!
			pShaderShadow->EnableTexture(SHADER_SAMPLER0, true);

			// Stock-Consistency
			if (params[GAMMACOLORREAD]->GetIntValue() == 0)
				pShaderShadow->EnableSRGBRead(SHADER_SAMPLER0, true);

			// DetailTexture ( s1 ), only sRGB when BlendMode != 0
			if (bHasDetailTexture)
			{
				pShaderShadow->EnableTexture(SHADER_SAMPLER1, true);

				// More Stock-Consistency
				if(nDetailBlendMode != 0)
					pShaderShadow->EnableSRGBRead(SHADER_SAMPLER1, true);
			}

			if (bHasAlphaTexture)
			{
				// Never sRGB!! Linear Values!!!
				pShaderShadow->EnableTexture(SHADER_SAMPLER2, true);
			}

			bool bDistanceAlphaFromDetail = false;
			bool bSoftMask = false;
			bool bGlow = false;
			bool bOutline = false;

			if (bHasDistanceAlpha)
			{
				bDistanceAlphaFromDetail = params[DISTANCEALPHAFROMDETAIL]->GetIntValue() != 0;
				bSoftMask = params[SOFTEDGES]->GetIntValue() != 0;
				bGlow = params[GLOW]->GetIntValue() != 0;
				bOutline = params[OUTLINE]->GetIntValue() != 0;
			}

			// 0 = Nothing
			// 1 = Detail ( Blendmode 0 )
			// 2 = Blendmode 1... and so on
			int nDetailTextureMode = bHasDetailTexture + nDetailBlendMode;

			// 0 = Nothing
			// 1 = Outline
			// 2 = SoftMask
			// 3 = Outline + SoftMask
			// 4 = Outer Glow
			// 5 = Outer Glow + Outline
			// 6 = Outer Glow + SoftMask
			// 7 = Outer Glow + Outline + SoftMask
			// 8+ = ^ that but DistanceAlphaFromDetail
			int nDistanceAlphaMode = 0;
			if (bHasDistanceAlpha)
			{
				// Tiny HackHack:
				// DistanceAlpha requires any of outline, softmask or glow
				// But you can set DistanceAlphaFromDetail regardless! oof!
				bDistanceAlphaFromDetail = bDistanceAlphaFromDetail && (bOutline || bSoftMask || bGlow);

				nDistanceAlphaMode += bOutline;
				nDistanceAlphaMode += bSoftMask * 2;
				nDistanceAlphaMode += bGlow * 4;
				nDistanceAlphaMode += bDistanceAlphaFromDetail * 7;
			}

			DECLARE_STATIC_VERTEX_SHADER(teamcolorhud_vs20);
			SET_STATIC_VERTEX_SHADER_COMBO(VERTEXCOLOR, bHasVertexRGB || bHasVertexA);
			SET_STATIC_VERTEX_SHADER_COMBO(DONT_GAMMA_CONVERT_VERTEX_COLOR, bSRGBWrite ? 0 : 1);
			SET_STATIC_VERTEX_SHADER_COMBO(DETAILTEXTURE, bHasDetailTexture);
			SET_STATIC_VERTEX_SHADER(teamcolorhud_vs20);

			DECLARE_STATIC_PIXEL_SHADER(teamcolorhud_ps20b);
			SET_STATIC_PIXEL_SHADER_COMBO(DISTANCEALPHAMODE, nDistanceAlphaMode);
			SET_STATIC_PIXEL_SHADER_COMBO(DETAILTEXTUREMODE, nDetailTextureMode);
			SET_STATIC_PIXEL_SHADER_COMBO(BLENDTINTBYXALPHA, bBlendTintByBaseAlpha);
			SET_STATIC_PIXEL_SHADER_COMBO(VERTEXRGB, bHasVertexRGB);
			SET_STATIC_PIXEL_SHADER_COMBO(VERTEXA, bHasVertexA);
			SET_STATIC_PIXEL_SHADER_COMBO(ALPHATEXTURE, bHasAlphaTexture);
			SET_STATIC_PIXEL_SHADER(teamcolorhud_ps20b);
		}

		//==========================================================================//
		// Dynamic State
		//==========================================================================//
		DYNAMIC_STATE
		{
			// No mat_fullbright support here, since we are using it on HUD Elements
			// s0
			if(bHasBaseTexture)
				BindTexture(SHADER_SAMPLER0, BASETEXTURE, FRAME);
			else
				pShaderAPI->BindStandardTexture(SHADER_SAMPLER0, TEXTURE_GREY_ALPHA_ZERO);

			SetVertexShaderTextureTransform(VERTEX_SHADER_SHADER_SPECIFIC_CONST_0, BASETEXTURETRANSFORM);

			// c0
			// ComputeModulationColor() sets an Alpha Value
			// Source: OrangeBox / ASW Code which have BaseShader.cpp
			Vector4D c0;
			ComputeModulationColor(c0.Base());

			// Valve moment
			c0.x = GammaToLinear(c0.x);
			c0.y = GammaToLinear(c0.y);
			c0.z = GammaToLinear(c0.z);

			c0.w = params[BLENDTINTCOLOROVERBASE]->GetFloatValue();
			pShaderAPI->SetPixelShaderConstant(0, c0.Base());

			if(bHasDetailTexture)
			{
				// s1
				BindTexture(SHADER_SAMPLER1, DETAIL, DETAILFRAME);

				// c1
				Vector4D c1;
				params[DETAILTINT]->GetVecValue(c1.Base(), 3);
				c1.w = params[DETAILBLENDFACTOR]->GetFloatValue();

				pShaderAPI->SetPixelShaderConstant(2, c1.Base());

				if(IsDefined(DETAILTEXTURETRANSFORM))
					SetVertexShaderTextureScaledTransform(VERTEX_SHADER_SHADER_SPECIFIC_CONST_4, DETAILTEXTURETRANSFORM, DETAILSCALE);
				else
					SetVertexShaderTextureScaledTransform(VERTEX_SHADER_SHADER_SPECIFIC_CONST_4, BASETEXTURETRANSFORM, DETAILSCALE);
			}

			// s2
			if (bHasAlphaTexture)
				BindTexture(SHADER_SAMPLER2, ALPHATEXTURE, ALPHATEXTUREFRAME);

			if (bHasDistanceAlpha)
			{
				float flSoftStart = params[EDGESOFTNESSSTART]->GetFloatValue();
				float flSoftEnd = params[EDGESOFTNESSEND]->GetFloatValue();
				// set all line art shader parms
				bool bScaleEdges = params[SCALEEDGESOFTNESSBASEDONSCREENRES]->GetIntValue() != 0;
				bool bScaleOutline = params[SCALEOUTLINESOFTNESSBASEDONSCREENRES]->GetIntValue() != 0;

				float flResScale = 1.0;

				float flOutlineStart0 = params[OUTLINESTART0]->GetFloatValue();
				float flOutlineStart1 = params[OUTLINESTART1]->GetFloatValue();
				float flOutlineEnd0 = params[OUTLINEEND0]->GetFloatValue();
				float flOutlineEnd1 = params[OUTLINEEND1]->GetFloatValue();

				if (bScaleEdges || bScaleOutline)
				{
					int nWidth, nHeight;
					pShaderAPI->GetBackBufferDimensions(nWidth, nHeight);
					flResScale = max(0.5f, max(1024.f / nWidth, 768.f / nHeight));

					if (bScaleEdges)
					{
						float flMid = 0.5 * (flSoftStart + flSoftEnd);
						flSoftStart = clamp(flMid + flResScale * (flSoftStart - flMid), 0.05, 0.99);
						flSoftEnd = clamp(flMid + flResScale * (flSoftEnd - flMid), 0.05, 0.99);
					}

					if (bScaleOutline)
					{
						// shrink the soft part of the outline, enlarging hard part
						float flMidS = 0.5 * (flOutlineStart1 + flOutlineStart0);
						flOutlineStart1 = clamp(flMidS + flResScale * (flOutlineStart1 - flMidS), 0.05, 0.99);
						float flMidE = 0.5 * (flOutlineEnd1 + flOutlineEnd0);
						flOutlineEnd1 = clamp(flMidE + flResScale * (flOutlineEnd1 - flMidE), 0.05, 0.99);
					}

				}

				float flConsts[] = {
					// c5 - glow values
					params[GLOWX]->GetFloatValue(),
					params[GLOWY]->GetFloatValue(),
					params[GLOWSTART]->GetFloatValue(),
					params[GLOWEND]->GetFloatValue(),
					// c6 - glow color
					0,0,0, // will be filled in
					params[GLOWALPHA]->GetFloatValue(),
					// c7 - mask range parms
					flSoftStart,
					flSoftEnd,
					0,0,
					// c8 - outline color
					0,0,0,
					params[OUTLINEALPHA]->GetFloatValue(),
					// c9 - outline parms. ordered for optimal ps20 .wzyx swizzling
					flOutlineStart0,
					flOutlineEnd1,
					flOutlineEnd0,
					flOutlineStart1,
				};

				params[GLOWCOLOR]->GetVecValue(flConsts + 4, 3);
				params[OUTLINECOLOR]->GetVecValue(flConsts + 12, 3);

				pShaderAPI->SetPixelShaderConstant(5, flConsts, 5);

			}

			DECLARE_DYNAMIC_VERTEX_SHADER(teamcolorhud_vs20);
			SET_DYNAMIC_VERTEX_SHADER_COMBO(SKINNING, pShaderAPI->GetCurrentNumBones() > 0);
			SET_DYNAMIC_VERTEX_SHADER_COMBO(COMPRESSED_VERTS, (int)vertexCompression);
			SET_DYNAMIC_VERTEX_SHADER(teamcolorhud_vs20);

			DECLARE_DYNAMIC_PIXEL_SHADER(teamcolorhud_ps20b);
			SET_DYNAMIC_PIXEL_SHADER(teamcolorhud_ps20b);
		}
		Draw();
	}
END_SHADER

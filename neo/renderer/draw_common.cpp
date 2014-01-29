/*
===========================================================================

Doom 3 GPL Source Code
Copyright (C) 1999-2011 id Software LLC, a ZeniMax Media company. 

This file is part of the Doom 3 GPL Source Code (?Doom 3 Source Code?).  

Doom 3 Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Doom 3 Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Doom 3 Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, the Doom 3 Source Code is also subject to certain additional terms. You should have received a copy of these additional terms immediately following the terms and conditions of the GNU General Public License which accompanied the Doom 3 Source Code.  If not, please request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc., Suite 120, Rockville, Maryland 20850 USA.

===========================================================================
*/
#include "precompiled.h"
#pragma hdrstop

#include "tr_local.h"

static const float zero[4] = { 0, 0, 0, 0 };
static const float one[4] = { 1, 1, 1, 1 };
static const float negOne[4] = { -1, -1, -1, -1 };

/*
================
RB_SetVertexColorParms
================
*/
static void RB_SetVertexColorParms( stageVertexColor_t svc ) {
	switch (svc) {
	case SVC_IGNORE:
		renderProgManager.SetRenderParm( RENDERPARM_VERTEXCOLOR_MODULATE, zero );
		renderProgManager.SetRenderParm( RENDERPARM_VERTEXCOLOR_ADD, one );
		break;
	case SVC_MODULATE:
		renderProgManager.SetRenderParm( RENDERPARM_VERTEXCOLOR_MODULATE, one );
		renderProgManager.SetRenderParm( RENDERPARM_VERTEXCOLOR_ADD, zero );
		break;
	case SVC_INVERSE_MODULATE:
		renderProgManager.SetRenderParm( RENDERPARM_VERTEXCOLOR_MODULATE, negOne );
		renderProgManager.SetRenderParm( RENDERPARM_VERTEXCOLOR_ADD, one );
		break;
	}
}

/*
=====================
RB_BakeTextureMatrixIntoTexgen
=====================
*/
void RB_BakeTextureMatrixIntoTexgen( idPlane lightProject[3], const float *textureMatrix ) {
	float	genMatrix[16];
	float	final[16];

	genMatrix[0] = lightProject[0][0];
	genMatrix[4] = lightProject[0][1];
	genMatrix[8] = lightProject[0][2];
	genMatrix[12] = lightProject[0][3];

	genMatrix[1] = lightProject[1][0];
	genMatrix[5] = lightProject[1][1];
	genMatrix[9] = lightProject[1][2];
	genMatrix[13] = lightProject[1][3];

	genMatrix[2] = 0;
	genMatrix[6] = 0;
	genMatrix[10] = 0;
	genMatrix[14] = 0;

	genMatrix[3] = lightProject[2][0];
	genMatrix[7] = lightProject[2][1];
	genMatrix[11] = lightProject[2][2];
	genMatrix[15] = lightProject[2][3];

	myGlMultMatrix( genMatrix, backEnd.lightTextureMatrix, final );

	lightProject[0][0] = final[0];
	lightProject[0][1] = final[4];
	lightProject[0][2] = final[8];
	lightProject[0][3] = final[12];

	lightProject[1][0] = final[1];
	lightProject[1][1] = final[5];
	lightProject[1][2] = final[9];
	lightProject[1][3] = final[13];
}

/*
================
RB_PrepareStageTexturing
================
*/
void RB_PrepareStageTexturing( const shaderStage_t *pStage,  const drawSurf_t *surf, idDrawVert *ac ) {
	// set privatePolygonOffset if necessary
	if ( pStage->privatePolygonOffset ) {
		qglEnable( GL_POLYGON_OFFSET_FILL );
		qglPolygonOffset( r_offsetFactor.GetFloat(), r_offsetUnits.GetFloat() * pStage->privatePolygonOffset );
	}

	// set the texture matrix if needed
	if ( pStage->texture.hasMatrix ) {
		RB_LoadShaderTextureMatrix( surf->shaderRegisters, &pStage->texture );
	}

	// texgens
	if ( pStage->texture.texgen == TG_DIFFUSE_CUBE ) {
		qglTexCoordPointer( 3, GL_FLOAT, sizeof( idDrawVert ), ac->normal.ToFloatPtr() );
	}
	if ( pStage->texture.texgen == TG_SKYBOX_CUBE || pStage->texture.texgen == TG_WOBBLESKY_CUBE ) {
		qglTexCoordPointer( 3, GL_FLOAT, 0, vertexCache.Position( surf->dynamicTexCoords ) );
	}
	if ( pStage->texture.texgen == TG_SCREEN ) {
		qglEnable( GL_TEXTURE_GEN_S );
		qglEnable( GL_TEXTURE_GEN_T );
		qglEnable( GL_TEXTURE_GEN_Q );

		float	mat[16], plane[4];
		myGlMultMatrix( surf->space->modelViewMatrix, backEnd.viewDef->projectionMatrix, mat );

		plane[0] = mat[0];
		plane[1] = mat[4];
		plane[2] = mat[8];
		plane[3] = mat[12];
		qglTexGenfv( GL_S, GL_OBJECT_PLANE, plane );

		plane[0] = mat[1];
		plane[1] = mat[5];
		plane[2] = mat[9];
		plane[3] = mat[13];
		qglTexGenfv( GL_T, GL_OBJECT_PLANE, plane );

		plane[0] = mat[3];
		plane[1] = mat[7];
		plane[2] = mat[11];
		plane[3] = mat[15];
		qglTexGenfv( GL_Q, GL_OBJECT_PLANE, plane );
	}

	if ( pStage->texture.texgen == TG_SCREEN2 ) {
		qglEnable( GL_TEXTURE_GEN_S );
		qglEnable( GL_TEXTURE_GEN_T );
		qglEnable( GL_TEXTURE_GEN_Q );

		float	mat[16], plane[4];
		myGlMultMatrix( surf->space->modelViewMatrix, backEnd.viewDef->projectionMatrix, mat );

		plane[0] = mat[0];
		plane[1] = mat[4];
		plane[2] = mat[8];
		plane[3] = mat[12];
		qglTexGenfv( GL_S, GL_OBJECT_PLANE, plane );

		plane[0] = mat[1];
		plane[1] = mat[5];
		plane[2] = mat[9];
		plane[3] = mat[13];
		qglTexGenfv( GL_T, GL_OBJECT_PLANE, plane );

		plane[0] = mat[3];
		plane[1] = mat[7];
		plane[2] = mat[11];
		plane[3] = mat[15];
		qglTexGenfv( GL_Q, GL_OBJECT_PLANE, plane );
	}

	if ( pStage->texture.texgen == TG_GLASSWARP && tr.backEndRenderer == BE_ARB2 ) {
		qglBindProgramARB( GL_FRAGMENT_PROGRAM_ARB, FPROG_GLASSWARP );
		qglEnable( GL_FRAGMENT_PROGRAM_ARB );

		GL_SelectTexture( 2 );
		globalImages->scratchImage->Bind();

		GL_SelectTexture( 1 );
		globalImages->scratchImage2->Bind();

		qglEnable( GL_TEXTURE_GEN_S );
		qglEnable( GL_TEXTURE_GEN_T );
		qglEnable( GL_TEXTURE_GEN_Q );

		float	mat[16], plane[4];
		myGlMultMatrix( surf->space->modelViewMatrix, backEnd.viewDef->projectionMatrix, mat );

		plane[0] = mat[0];
		plane[1] = mat[4];
		plane[2] = mat[8];
		plane[3] = mat[12];
		qglTexGenfv( GL_S, GL_OBJECT_PLANE, plane );

		plane[0] = mat[1];
		plane[1] = mat[5];
		plane[2] = mat[9];
		plane[3] = mat[13];
		qglTexGenfv( GL_T, GL_OBJECT_PLANE, plane );

		plane[0] = mat[3];
		plane[1] = mat[7];
		plane[2] = mat[11];
		plane[3] = mat[15];
		qglTexGenfv( GL_Q, GL_OBJECT_PLANE, plane );

		GL_SelectTexture( 0 );
	}

	if ( pStage->texture.texgen == TG_REFLECT_CUBE && tr.backEndRenderer == BE_ARB2 ) {
		// see if there is also a bump map specified
		const shaderStage_t *bumpStage = surf->material->GetBumpStage();
		if ( bumpStage ) {
			// per-pixel reflection mapping with bump mapping
			GL_SelectTexture( 1 );
			bumpStage->texture.image->Bind();
			GL_SelectTexture( 0 );

			qglNormalPointer( GL_FLOAT, sizeof( idDrawVert ), ac->normal.ToFloatPtr() );
			qglVertexAttribPointerARB( 10, 3, GL_FLOAT, false, sizeof( idDrawVert ), ac->tangents[1].ToFloatPtr() );
			qglVertexAttribPointerARB( 9, 3, GL_FLOAT, false, sizeof( idDrawVert ), ac->tangents[0].ToFloatPtr() );

			qglEnableVertexAttribArrayARB( 9 );
			qglEnableVertexAttribArrayARB( 10 );
			qglEnableClientState( GL_NORMAL_ARRAY );

			// Program env 5, 6, 7, 8 have been set in RB_SetProgramEnvironmentSpace

			qglBindProgramARB( GL_FRAGMENT_PROGRAM_ARB, FPROG_BUMPY_ENVIRONMENT );
			qglEnable( GL_FRAGMENT_PROGRAM_ARB );
			qglBindProgramARB( GL_VERTEX_PROGRAM_ARB, VPROG_BUMPY_ENVIRONMENT );
			qglEnable( GL_VERTEX_PROGRAM_ARB );
		} else {
			// per-pixel reflection mapping without a normal map
			qglNormalPointer( GL_FLOAT, sizeof( idDrawVert ), ac->normal.ToFloatPtr() );
			qglEnableClientState( GL_NORMAL_ARRAY );

			qglBindProgramARB( GL_FRAGMENT_PROGRAM_ARB, FPROG_ENVIRONMENT );
			qglEnable( GL_FRAGMENT_PROGRAM_ARB );
			qglBindProgramARB( GL_VERTEX_PROGRAM_ARB, VPROG_ENVIRONMENT );
			qglEnable( GL_VERTEX_PROGRAM_ARB );
		}
	}
}

/*
================
RB_FinishStageTexturing
================
*/
void RB_FinishStageTexturing( const shaderStage_t *pStage, const drawSurf_t *surf, idDrawVert *ac ) {
	// unset privatePolygonOffset if necessary
	if ( pStage->privatePolygonOffset && !surf->material->TestMaterialFlag(MF_POLYGONOFFSET) ) {
		qglDisable( GL_POLYGON_OFFSET_FILL );
	}

	if ( pStage->texture.texgen == TG_DIFFUSE_CUBE || pStage->texture.texgen == TG_SKYBOX_CUBE
		|| pStage->texture.texgen == TG_WOBBLESKY_CUBE ) {
		qglTexCoordPointer( 2, GL_FLOAT, sizeof( idDrawVert ), (void *)&ac->st );
	}

	if ( pStage->texture.texgen == TG_SCREEN ) {
		qglDisable( GL_TEXTURE_GEN_S );
		qglDisable( GL_TEXTURE_GEN_T );
		qglDisable( GL_TEXTURE_GEN_Q );
	}
	if ( pStage->texture.texgen == TG_SCREEN2 ) {
		qglDisable( GL_TEXTURE_GEN_S );
		qglDisable( GL_TEXTURE_GEN_T );
		qglDisable( GL_TEXTURE_GEN_Q );
	}

	if ( pStage->texture.texgen == TG_GLASSWARP && tr.backEndRenderer == BE_ARB2 ) {
		GL_SelectTexture( 2 );
		globalImages->BindNull();

		GL_SelectTexture( 1 );
		if ( pStage->texture.hasMatrix ) {
			RB_LoadShaderTextureMatrix( surf->shaderRegisters, &pStage->texture );
		}
		qglDisable( GL_TEXTURE_GEN_S );
		qglDisable( GL_TEXTURE_GEN_T );
		qglDisable( GL_TEXTURE_GEN_Q );
		qglDisable( GL_FRAGMENT_PROGRAM_ARB );
		globalImages->BindNull();
		GL_SelectTexture( 0 );
	}

	if ( pStage->texture.texgen == TG_REFLECT_CUBE && tr.backEndRenderer == BE_ARB2 ) {
		// see if there is also a bump map specified
		const shaderStage_t *bumpStage = surf->material->GetBumpStage();
		if ( bumpStage ) {
			// per-pixel reflection mapping with bump mapping
			GL_SelectTexture( 1 );
			globalImages->BindNull();
			GL_SelectTexture( 0 );

			qglDisableVertexAttribArrayARB( 9 );
			qglDisableVertexAttribArrayARB( 10 );
		} else {
			// per-pixel reflection mapping without bump mapping
		}

		qglDisableClientState( GL_NORMAL_ARRAY );
		qglDisable( GL_FRAGMENT_PROGRAM_ARB );
		qglDisable( GL_VERTEX_PROGRAM_ARB );
	}

	if ( pStage->texture.hasMatrix ) {
		qglMatrixMode( GL_TEXTURE );
		qglLoadIdentity();
		qglMatrixMode( GL_MODELVIEW );
	}
}

/*
=============================================================================================

FILL DEPTH BUFFER

=============================================================================================
*/


/*
==================
RB_T_FillDepthBuffer
==================
*/
void RB_T_FillDepthBuffer( const drawSurf_t *surf ) {
	int			stage;
	const idMaterial	*shader;
	const shaderStage_t *pStage;
	const float	*regs;
	float		color[4];
	const srfTriangles_t	*tri;

	tri = surf->geo;
	shader = surf->material;

	if ( !shader->IsDrawn() ) {
		return;
	}

	// some deforms may disable themselves by setting numIndexes = 0
	if ( !tri->numIndexes ) {
		return;
	}

	// translucent surfaces don't put anything in the depth buffer and don't
	// test against it, which makes them fail the mirror clip plane operation
	if ( shader->Coverage() == MC_TRANSLUCENT ) {
		return;
	}

	if ( !tri->ambientCache ) {
		common->Printf( "RB_T_FillDepthBuffer: !tri->ambientCache\n" );
		return;
	}

	// get the expressions for conditionals / color / texcoords
	regs = surf->shaderRegisters;

	// if all stages of a material have been conditioned off, don't do anything
	for ( stage = 0; stage < shader->GetNumStages() ; stage++ ) {		
		pStage = shader->GetStage(stage);
		// check the stage enable condition
		if ( regs[ pStage->conditionRegister ] != 0 ) {
			break;
		}
	}
	if ( stage == shader->GetNumStages() ) {
		return;
	}

	// set polygon offset if necessary
	if ( shader->TestMaterialFlag(MF_POLYGONOFFSET) ) {
		qglEnable( GL_POLYGON_OFFSET_FILL );
		qglPolygonOffset( r_offsetFactor.GetFloat(), r_offsetUnits.GetFloat() * shader->GetPolygonOffset() );
	}

	// subviews will just down-modulate the color buffer by overbright
	if ( shader->GetSort() == SS_SUBVIEW ) {
		GL_State( GLS_SRCBLEND_DST_COLOR | GLS_DSTBLEND_ZERO | GLS_DEPTHFUNC_LESS );
		color[0] = 1.0f;
		color[1] = 1.0f;
		color[2] = 1.0f;
		color[3] = 1.0f;
	} else {
		// others just draw black
		color[0] = 0.0f;
		color[1] = 0.0f;
		color[2] = 0.0f;
		color[3] = 1.0f;
	}

	idDrawVert *ac = (idDrawVert *)vertexCache.Position( tri->ambientCache );
	qglVertexPointer( 3, GL_FLOAT, sizeof( idDrawVert ), ac->xyz.ToFloatPtr() );
	qglTexCoordPointer( 2, GL_FLOAT, sizeof( idDrawVert ), reinterpret_cast<void *>(&ac->st) );

	bool drawSolid = false;

	if ( shader->Coverage() == MC_OPAQUE ) {
		drawSolid = true;
	} else if ( shader->Coverage() == MC_PERFORATED ) {
		// we may have multiple alpha tested stages
		// if the only alpha tested stages are condition register omitted,
		// draw a normal opaque surface
		bool	didDraw = false;

		// perforated surfaces may have multiple alpha tested stages
		for ( stage = 0; stage < shader->GetNumStages() ; stage++ ) {		
			pStage = shader->GetStage(stage);

			if ( !pStage->hasAlphaTest ) {
				continue;
			}

			// check the stage enable condition
			if ( regs[ pStage->conditionRegister ] == 0 ) {
				continue;
			}

			// if we at least tried to draw an alpha tested stage,
			// we won't draw the opaque surface
			didDraw = true;

			// set the alpha modulate
			color[3] = regs[ pStage->color.registers[3] ];

			// skip the entire stage if alpha would be black
			if ( color[3] <= 0 ) {
				continue;
			}

			renderProgManager.SetRenderParm( RENDERPARM_COLOR, color );
			renderProgManager.SetRenderParm( RENDERPARM_ALPHA_TEST, &regs[ pStage->alphaTestRegister ] );
			RB_SetVertexColorParms( SVC_IGNORE );

			// bind the texture
			pStage->texture.image->Bind();

			// set texture matrix and texGens
			RB_PrepareStageTexturing( pStage, surf, ac );

			// draw it
			RB_DrawElementsWithCounters( tri );

			// clean up
			RB_FinishStageTexturing( pStage, surf, ac );
		}

		if ( !didDraw ) {
			drawSolid = true;
		}
	}

	// draw the entire surface solid
	if ( drawSolid ) {
		renderProgManager.SetRenderParm( RENDERPARM_COLOR, color );
		renderProgManager.SetRenderParm( RENDERPARM_ALPHA_TEST, vec4_zero.ToFloatPtr() );

		globalImages->whiteImage->Bind();

		// draw it
		RB_DrawElementsWithCounters( tri );
	}

	// reset polygon offset
	if ( shader->TestMaterialFlag(MF_POLYGONOFFSET) ) {
		qglDisable( GL_POLYGON_OFFSET_FILL );
	}

	// reset blending
	if ( shader->GetSort() == SS_SUBVIEW ) {
		GL_State( GLS_DEPTHFUNC_LESS );
	}
}

/*
=====================
RB_STD_FillDepthBuffer

If we are rendering a subview with a near clip plane, use a second texture
to force the alpha test to fail when behind that clip plane
=====================
*/
void RB_STD_FillDepthBuffer( drawSurf_t **drawSurfs, int numDrawSurfs ) {
	// if we are just doing 2D rendering, no need to fill the depth buffer
	if ( !backEnd.viewDef->viewEntitys ) {
		return;
	}

	renderLog.OpenMainBlock( MRB_FILL_DEPTH_BUFFER );
	renderLog.OpenBlock( "RB_FillDepthBuffer" );

	renderProgManager.BindShader_Depth();

	// the first texture will be used for alpha tested surfaces
	GL_SelectTexture( 0 );
	qglEnableClientState( GL_TEXTURE_COORD_ARRAY );

	// decal surfaces may enable polygon offset
	qglPolygonOffset( r_offsetFactor.GetFloat(), r_offsetUnits.GetFloat() );

	GL_State( GLS_DEPTHFUNC_LESS );

	// Enable stencil test if we are going to be using it for shadows.
	// If we didn't do this, it would be legal behavior to get z fighting
	// from the ambient pass and the light passes.
	qglEnable( GL_STENCIL_TEST );
	qglStencilFunc( GL_ALWAYS, 1, 255 );

	RB_RenderDrawSurfListWithFunction( drawSurfs, numDrawSurfs, RB_T_FillDepthBuffer );

	renderProgManager.Unbind();

	renderLog.CloseBlock();
	renderLog.CloseMainBlock();
}

/*
=============================================================================================

SHADER PASSES

=============================================================================================
*/

/*
==================
RB_SetProgramEnvironment

Sets variables that can be used by all vertex programs
==================
*/
void RB_SetProgramEnvironment( void ) {
	float	parm[4];
	int		pot;

	if ( !glConfig.ARBVertexProgramAvailable ) {
		return;
	}

#if 0
	// screen power of two correction factor, one pixel in so we don't get a bilerp
	// of an uncopied pixel
	int	 w = backEnd.viewDef->viewport.x2 - backEnd.viewDef->viewport.x1 + 1;
	pot = globalImages->currentRenderImage->uploadWidth;
	if ( w == pot ) {
		parm[0] = 1.0;
	} else {
		parm[0] = (float)(w-1) / pot;
	}

	int	 h = backEnd.viewDef->viewport.y2 - backEnd.viewDef->viewport.y1 + 1;
	pot = globalImages->currentRenderImage->uploadHeight;
	if ( h == pot ) {
		parm[1] = 1.0;
	} else {
		parm[1] = (float)(h-1) / pot;
	}

	parm[2] = 0;
	parm[3] = 1;
	qglProgramEnvParameter4fvARB( GL_VERTEX_PROGRAM_ARB, 0, parm );
#else
	// screen power of two correction factor, assuming the copy to _currentRender
	// also copied an extra row and column for the bilerp
	int	 w = backEnd.viewDef->viewport.x2 - backEnd.viewDef->viewport.x1 + 1;
	pot = globalImages->currentRenderImage->uploadWidth;
	parm[0] = (float)w / pot;

	int	 h = backEnd.viewDef->viewport.y2 - backEnd.viewDef->viewport.y1 + 1;
	pot = globalImages->currentRenderImage->uploadHeight;
	parm[1] = (float)h / pot;

	parm[2] = 0;
	parm[3] = 1;
	qglProgramEnvParameter4fvARB( GL_VERTEX_PROGRAM_ARB, 0, parm );
#endif

	qglProgramEnvParameter4fvARB( GL_FRAGMENT_PROGRAM_ARB, 0, parm );

	// window coord to 0.0 to 1.0 conversion
	parm[0] = 1.0 / w;
	parm[1] = 1.0 / h;
	parm[2] = 0;
	parm[3] = 1;
	qglProgramEnvParameter4fvARB( GL_FRAGMENT_PROGRAM_ARB, 1, parm );

	//
	// set eye position in global space
	//
	parm[0] = backEnd.viewDef->renderView.vieworg[0];
	parm[1] = backEnd.viewDef->renderView.vieworg[1];
	parm[2] = backEnd.viewDef->renderView.vieworg[2];
	parm[3] = 1.0;
	qglProgramEnvParameter4fvARB( GL_VERTEX_PROGRAM_ARB, 1, parm );


}

/*
==================
RB_SetProgramEnvironmentSpace

Sets variables related to the current space that can be used by all vertex programs
==================
*/
void RB_SetProgramEnvironmentSpace( void ) {
	if ( !glConfig.ARBVertexProgramAvailable ) {
		return;
	}

	const struct viewEntity_s *space = backEnd.currentSpace;
	float	parm[4];

	// set eye position in local space
	R_GlobalPointToLocal( space->modelMatrix, backEnd.viewDef->renderView.vieworg, *(idVec3 *)parm );
	parm[3] = 1.0;
	qglProgramEnvParameter4fvARB( GL_VERTEX_PROGRAM_ARB, 5, parm );

	// we need the model matrix without it being combined with the view matrix
	// so we can transform local vectors to global coordinates
	parm[0] = space->modelMatrix[0];
	parm[1] = space->modelMatrix[4];
	parm[2] = space->modelMatrix[8];
	parm[3] = space->modelMatrix[12];
	qglProgramEnvParameter4fvARB( GL_VERTEX_PROGRAM_ARB, 6, parm );
	parm[0] = space->modelMatrix[1];
	parm[1] = space->modelMatrix[5];
	parm[2] = space->modelMatrix[9];
	parm[3] = space->modelMatrix[13];
	qglProgramEnvParameter4fvARB( GL_VERTEX_PROGRAM_ARB, 7, parm );
	parm[0] = space->modelMatrix[2];
	parm[1] = space->modelMatrix[6];
	parm[2] = space->modelMatrix[10];
	parm[3] = space->modelMatrix[14];
	qglProgramEnvParameter4fvARB( GL_VERTEX_PROGRAM_ARB, 8, parm );
}

/*
==================
RB_STD_T_RenderShaderPasses

This is also called for the generated 2D rendering
==================
*/
void RB_STD_T_RenderShaderPasses( const drawSurf_t *surf ) {
	int			stage;
	const idMaterial	*shader;
	const shaderStage_t *pStage;
	const float	*regs;
	float		color[4];
	const srfTriangles_t	*tri;

	tri = surf->geo;
	shader = surf->material;

	if ( !shader->HasAmbient() ) {
		return;
	}

	if ( shader->IsPortalSky() ) {
		return;
	}

	// change the matrix if needed
	if ( surf->space != backEnd.currentSpace ) {
		qglLoadMatrixf( surf->space->modelViewMatrix );
		backEnd.currentSpace = surf->space;
		RB_SetProgramEnvironmentSpace();
	}

	// change the scissor if needed
	if ( r_useScissor.GetBool() && !backEnd.currentScissor.Equals( surf->scissorRect ) ) {
		backEnd.currentScissor = surf->scissorRect;
		qglScissor( backEnd.viewDef->viewport.x1 + backEnd.currentScissor.x1, 
			backEnd.viewDef->viewport.y1 + backEnd.currentScissor.y1,
			backEnd.currentScissor.x2 + 1 - backEnd.currentScissor.x1,
			backEnd.currentScissor.y2 + 1 - backEnd.currentScissor.y1 );
	}

	// some deforms may disable themselves by setting numIndexes = 0
	if ( !tri->numIndexes ) {
		return;
	}

	if ( !tri->ambientCache ) {
		common->Printf( "RB_T_RenderShaderPasses: !tri->ambientCache\n" );
		return;
	}

	// get the expressions for conditionals / color / texcoords
	regs = surf->shaderRegisters;

	// set face culling appropriately
	GL_Cull( shader->GetCullType() );

	// set polygon offset if necessary
	if ( shader->TestMaterialFlag(MF_POLYGONOFFSET) ) {
		qglEnable( GL_POLYGON_OFFSET_FILL );
		qglPolygonOffset( r_offsetFactor.GetFloat(), r_offsetUnits.GetFloat() * shader->GetPolygonOffset() );
	}
	
	if ( surf->space->weaponDepthHack ) {
		RB_EnterWeaponDepthHack( surf );
	}

	if ( surf->space->modelDepthHack != 0.0f ) {
		RB_EnterModelDepthHack( surf );
	}

	idDrawVert *ac = (idDrawVert *)vertexCache.Position( tri->ambientCache );
	qglVertexPointer( 3, GL_FLOAT, sizeof( idDrawVert ), ac->xyz.ToFloatPtr() );
	qglTexCoordPointer( 2, GL_FLOAT, sizeof( idDrawVert ), reinterpret_cast<void *>(&ac->st) );

	for ( stage = 0; stage < shader->GetNumStages() ; stage++ ) {		
		pStage = shader->GetStage(stage);

		// check the enable condition
		if ( regs[ pStage->conditionRegister ] == 0 ) {
			continue;
		}

		// skip the stages involved in lighting
		if ( pStage->lighting != SL_AMBIENT ) {
			continue;
		}

		// skip if the stage is ( GL_ZERO, GL_ONE ), which is used for some alpha masks
		if ( ( pStage->drawStateBits & (GLS_SRCBLEND_BITS|GLS_DSTBLEND_BITS) ) == ( GLS_SRCBLEND_ZERO | GLS_DSTBLEND_ONE ) ) {
			continue;
		}

		// see if we are a new-style stage
		newShaderStage_t *newStage = pStage->newStage;
		if ( newStage ) {
			//--------------------------
			//
			// new style stages
			//
			//--------------------------
			if ( r_skipNewAmbient.GetBool() ) {
				continue;
			}
			qglColorPointer( 4, GL_UNSIGNED_BYTE, sizeof( idDrawVert ), (void *)&ac->color );
			qglVertexAttribPointerARB( 9, 3, GL_FLOAT, false, sizeof( idDrawVert ), ac->tangents[0].ToFloatPtr() );
			qglVertexAttribPointerARB( 10, 3, GL_FLOAT, false, sizeof( idDrawVert ), ac->tangents[1].ToFloatPtr() );
			qglNormalPointer( GL_FLOAT, sizeof( idDrawVert ), ac->normal.ToFloatPtr() );

			qglEnableClientState( GL_COLOR_ARRAY );
			qglEnableVertexAttribArrayARB( 9 );
			qglEnableVertexAttribArrayARB( 10 );
			qglEnableClientState( GL_NORMAL_ARRAY );

			GL_State( pStage->drawStateBits );
			
			qglBindProgramARB( GL_VERTEX_PROGRAM_ARB, newStage->vertexProgram );
			qglEnable( GL_VERTEX_PROGRAM_ARB );

			// megaTextures bind a lot of images and set a lot of parameters
			if ( newStage->megaTexture ) {
				newStage->megaTexture->SetMappingForSurface( tri );
				idVec3	localViewer;
				R_GlobalPointToLocal( surf->space->modelMatrix, backEnd.viewDef->renderView.vieworg, localViewer );
				newStage->megaTexture->BindForViewOrigin( localViewer );
			}

			for ( int i = 0 ; i < newStage->numVertexParms ; i++ ) {
				float	parm[4];
				parm[0] = regs[ newStage->vertexParms[i][0] ];
				parm[1] = regs[ newStage->vertexParms[i][1] ];
				parm[2] = regs[ newStage->vertexParms[i][2] ];
				parm[3] = regs[ newStage->vertexParms[i][3] ];
				qglProgramLocalParameter4fvARB( GL_VERTEX_PROGRAM_ARB, i, parm );
			}

			for ( int i = 0 ; i < newStage->numFragmentProgramImages ; i++ ) {
				if ( newStage->fragmentProgramImages[i] ) {
					GL_SelectTexture( i );
					newStage->fragmentProgramImages[i]->Bind();
				}
			}
			qglBindProgramARB( GL_FRAGMENT_PROGRAM_ARB, newStage->fragmentProgram );
			qglEnable( GL_FRAGMENT_PROGRAM_ARB );

			// draw it
			RB_DrawElementsWithCounters( tri );

			for ( int i = 1 ; i < newStage->numFragmentProgramImages ; i++ ) {
				if ( newStage->fragmentProgramImages[i] ) {
					GL_SelectTexture( i );
					globalImages->BindNull();
				}
			}
			if ( newStage->megaTexture ) {
				newStage->megaTexture->Unbind();
			}

			GL_SelectTexture( 0 );

			qglDisable( GL_VERTEX_PROGRAM_ARB );
			qglDisable( GL_FRAGMENT_PROGRAM_ARB );

			qglDisableClientState( GL_COLOR_ARRAY );
			qglDisableVertexAttribArrayARB( 9 );
			qglDisableVertexAttribArrayARB( 10 );
			qglDisableClientState( GL_NORMAL_ARRAY );
			continue;
		}

		//--------------------------
		//
		// old style stages
		//
		//--------------------------

		// set the color
		color[0] = regs[ pStage->color.registers[0] ];
		color[1] = regs[ pStage->color.registers[1] ];
		color[2] = regs[ pStage->color.registers[2] ];
		color[3] = regs[ pStage->color.registers[3] ];

		// skip the entire stage if an add would be black
		if ( ( pStage->drawStateBits & (GLS_SRCBLEND_BITS|GLS_DSTBLEND_BITS) ) == ( GLS_SRCBLEND_ONE | GLS_DSTBLEND_ONE ) 
			&& color[0] <= 0 && color[1] <= 0 && color[2] <= 0 ) {
			continue;
		}

		// skip the entire stage if a blend would be completely transparent
		if ( ( pStage->drawStateBits & (GLS_SRCBLEND_BITS|GLS_DSTBLEND_BITS) ) == ( GLS_SRCBLEND_SRC_ALPHA | GLS_DSTBLEND_ONE_MINUS_SRC_ALPHA )
			&& color[3] <= 0 ) {
			continue;
		}

		// select the vertex color source
		if ( pStage->vertexColor == SVC_IGNORE ) {
			qglColor4fv( color );
		} else {
			qglColorPointer( 4, GL_UNSIGNED_BYTE, sizeof( idDrawVert ), (void *)&ac->color );
			qglEnableClientState( GL_COLOR_ARRAY );

			if ( pStage->vertexColor == SVC_INVERSE_MODULATE ) {
				GL_TexEnv( GL_COMBINE_ARB );
				qglTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_MODULATE );
				qglTexEnvi( GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE );
				qglTexEnvi( GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PRIMARY_COLOR_ARB );
				qglTexEnvi( GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB, GL_SRC_COLOR );
				qglTexEnvi( GL_TEXTURE_ENV, GL_OPERAND1_RGB_ARB, GL_ONE_MINUS_SRC_COLOR );
				qglTexEnvi( GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 1 );
			}

			// for vertex color and modulated color, we need to enable a second
			// texture stage
			if ( color[0] != 1 || color[1] != 1 || color[2] != 1 || color[3] != 1 ) {
				GL_SelectTexture( 1 );

				globalImages->whiteImage->Bind();
				GL_TexEnv( GL_COMBINE_ARB );

				qglTexEnvfv( GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, color );

				qglTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_MODULATE );
				qglTexEnvi( GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_PREVIOUS_ARB );
				qglTexEnvi( GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_CONSTANT_ARB );
				qglTexEnvi( GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB, GL_SRC_COLOR );
				qglTexEnvi( GL_TEXTURE_ENV, GL_OPERAND1_RGB_ARB, GL_SRC_COLOR );
				qglTexEnvi( GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 1 );

				qglTexEnvi( GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_MODULATE );
				qglTexEnvi( GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_PREVIOUS_ARB );
				qglTexEnvi( GL_TEXTURE_ENV, GL_SOURCE1_ALPHA_ARB, GL_CONSTANT_ARB );
				qglTexEnvi( GL_TEXTURE_ENV, GL_OPERAND0_ALPHA_ARB, GL_SRC_ALPHA );
				qglTexEnvi( GL_TEXTURE_ENV, GL_OPERAND1_ALPHA_ARB, GL_SRC_ALPHA );
				qglTexEnvi( GL_TEXTURE_ENV, GL_ALPHA_SCALE, 1 );

				GL_SelectTexture( 0 );
			}
		}

		// bind the texture
		RB_BindVariableStageImage( &pStage->texture, regs );

		// set the state
		GL_State( pStage->drawStateBits );
		
		RB_PrepareStageTexturing( pStage, surf, ac );

		// draw it
		RB_DrawElementsWithCounters( tri );

		RB_FinishStageTexturing( pStage, surf, ac );
		
		if ( pStage->vertexColor != SVC_IGNORE ) {
			qglDisableClientState( GL_COLOR_ARRAY );

			GL_SelectTexture( 1 );
			GL_TexEnv( GL_MODULATE );
			globalImages->BindNull();
			GL_SelectTexture( 0 );
			GL_TexEnv( GL_MODULATE );
		}
	}

	// reset polygon offset
	if ( shader->TestMaterialFlag(MF_POLYGONOFFSET) ) {
		qglDisable( GL_POLYGON_OFFSET_FILL );
	}
	if ( surf->space->weaponDepthHack || surf->space->modelDepthHack != 0.0f ) {
		RB_LeaveDepthHack( surf );
	}
}

/*
=====================
RB_STD_DrawShaderPasses

Draw non-light dependent passes
=====================
*/
int RB_STD_DrawShaderPasses( drawSurf_t **drawSurfs, int numDrawSurfs ) {
	int				i;

	// only obey skipAmbient if we are rendering a view
	if ( backEnd.viewDef->viewEntitys && r_skipAmbient.GetBool() ) {
		return numDrawSurfs;
	}

	RENDERLOG_PRINTF("---------- RB_STD_DrawShaderPasses ----------\n");

	// if we are about to draw the first surface that needs
	// the rendering in a texture, copy it over
	if ( drawSurfs[0]->material->GetSort() >= SS_POST_PROCESS ) {
		if ( r_skipPostProcess.GetBool() ) {
			return 0;
		}

		// only dump if in a 3d view
		if ( backEnd.viewDef->viewEntitys ) {
			globalImages->currentRenderImage->CopyFramebuffer( backEnd.viewDef->viewport.x1,
				backEnd.viewDef->viewport.y1,  backEnd.viewDef->viewport.x2 -  backEnd.viewDef->viewport.x1 + 1,
				backEnd.viewDef->viewport.y2 -  backEnd.viewDef->viewport.y1 + 1, true );
		}
		backEnd.currentRenderCopied = true;
	}

	GL_SelectTexture( 1 );
	globalImages->BindNull();

	GL_SelectTexture( 0 );
	qglEnableClientState( GL_TEXTURE_COORD_ARRAY );

	RB_SetProgramEnvironment();

	// we don't use RB_RenderDrawSurfListWithFunction()
	// because we want to defer the matrix load because many
	// surfaces won't draw any ambient passes
	backEnd.currentSpace = NULL;
	for (i = 0  ; i < numDrawSurfs ; i++ ) {
		if ( drawSurfs[i]->material->SuppressInSubview() ) {
			continue;
		}

		if ( backEnd.viewDef->isXraySubview && drawSurfs[i]->space->entityDef ) {
			if ( drawSurfs[i]->space->entityDef->parms.xrayIndex != 2 ) {
				continue;
			}
		}

		// we need to draw the post process shaders after we have drawn the fog lights
		if ( drawSurfs[i]->material->GetSort() >= SS_POST_PROCESS
			&& !backEnd.currentRenderCopied ) {
			break;
		}

		RB_STD_T_RenderShaderPasses( drawSurfs[i] );
	}

	GL_Cull( CT_FRONT_SIDED );
	qglColor3f( 1, 1, 1 );

	return i;
}



/*
==============================================================================

BACK END RENDERING OF STENCIL SHADOWS

==============================================================================
*/

/*
=====================
RB_T_Shadow

the shadow volumes face INSIDE
=====================
*/
static void RB_T_Shadow( const drawSurf_t *surf ) {
	const srfTriangles_t	*tri;

	// set the light position to project the rear surfaces
	if ( surf->space != backEnd.currentSpace ) {
		idVec4 localLight;

		R_GlobalPointToLocal( surf->space->modelMatrix, backEnd.vLight->globalLightOrigin, localLight.ToVec3() );
		localLight.w = 0.0f;

		renderProgManager.SetRenderParm( RENDERPARM_LOCALLIGHTORIGIN, localLight.ToFloatPtr() );

		float mat[16];
		myGlMultMatrix( surf->space->modelViewMatrix, backEnd.viewDef->projectionMatrix, mat );
		renderProgManager.SetRenderParms( RENDERPARM_MVPMATRIX_X, mat, 4 );
	}

	tri = surf->geo;

	if ( !tri->shadowCache ) {
		return;
	}

	qglVertexPointer( 4, GL_FLOAT, sizeof( shadowCache_t ), vertexCache.Position(tri->shadowCache) );

	// we always draw the sil planes, but we may not need to draw the front or rear caps
	int	numIndexes;
	bool external = false;

	if ( !r_useExternalShadows.GetInteger() ) {
		numIndexes = tri->numIndexes;
	} else if ( r_useExternalShadows.GetInteger() == 2 ) { // force to no caps for testing
		numIndexes = tri->numShadowIndexesNoCaps;
	} else if ( !(surf->dsFlags & DSF_VIEW_INSIDE_SHADOW) ) { 
		// if we aren't inside the shadow projection, no caps are ever needed needed
		numIndexes = tri->numShadowIndexesNoCaps;
		external = true;
	} else if ( !backEnd.vLight->viewInsideLight && !(surf->geo->shadowCapPlaneBits & SHADOW_CAP_INFINITE) ) {
		// if we are inside the shadow projection, but outside the light, and drawing
		// a non-infinite shadow, we can skip some caps
		if ( backEnd.vLight->viewSeesShadowPlaneBits & surf->geo->shadowCapPlaneBits ) {
			// we can see through a rear cap, so we need to draw it, but we can skip the
			// caps on the actual surface
			numIndexes = tri->numShadowIndexesNoFrontCaps;
		} else {
			// we don't need to draw any caps
			numIndexes = tri->numShadowIndexesNoCaps;
		}
		external = true;
	} else {
		// must draw everything
		numIndexes = tri->numIndexes;
	}

	// set depth bounds
	if( glConfig.depthBoundsTestAvailable && r_useDepthBoundsTest.GetBool() ) {
		qglDepthBoundsEXT( surf->scissorRect.zmin, surf->scissorRect.zmax );
	}

	// debug visualization
	if ( r_showShadows.GetInteger() ) {
		idVec4 debugColor = idVec4( 0.0f, 0.0f, 0.0f, 0.0f );
		if ( r_showShadows.GetInteger() == 3 ) {
			if ( external ) {
				debugColor = idVec4( 0.1f, 1.0f, 0.1f, 1.0f );
			} else {
				// these are the surfaces that require the reverse
				debugColor = idVec4( 1.0f, 0.1f, 0.1f, 1.0f );
			}
		} else {
			// draw different color for turboshadows
			if ( surf->geo->shadowCapPlaneBits & SHADOW_CAP_INFINITE ) {
				if ( numIndexes == tri->numIndexes ) {
					debugColor = idVec4( 1.0f, 0.1f, 0.1f, 1.0f );
				} else {
					debugColor = idVec4( 1.0f, 0.4f, 0.1f, 1.0f );
				}
			} else {
				if ( numIndexes == tri->numIndexes ) {
					debugColor = idVec4( 0.1f, 1.0f, 0.1f, 1.0f );
				} else if ( numIndexes == tri->numShadowIndexesNoFrontCaps ) {
					debugColor = idVec4( 0.1f, 1.0f, 0.6f, 1.0f );
				} else {
					debugColor = idVec4( 0.6f, 1.0f, 0.1f, 1.0f );
				}
			}
		}

		renderProgManager.SetRenderParm( RENDERPARM_COLOR, debugColor.ToFloatPtr() );

		renderProgManager.CommitUniforms();

		qglStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
		qglDisable( GL_STENCIL_TEST );

		GL_Cull( CT_TWO_SIDED );

		RB_DrawShadowElementsWithCounters( tri, numIndexes );

		GL_Cull( CT_FRONT_SIDED );

		qglEnable( GL_STENCIL_TEST );

		return;
	}

	renderProgManager.CommitUniforms();

	// patent-free work around
	if ( !external ) {
		// depth-fail stencil shadows
		if( r_useTwoSidedStencil.GetBool() && glConfig.twoSidedStencilAvailable ) {
			qglStencilOpSeparate( backEnd.viewDef->isMirror ? GL_FRONT : GL_BACK, GL_KEEP, tr.stencilDecr, GL_KEEP );
			qglStencilOpSeparate( backEnd.viewDef->isMirror ? GL_BACK : GL_FRONT, GL_KEEP, tr.stencilIncr, GL_KEEP );
			GL_Cull( CT_TWO_SIDED );
			RB_DrawShadowElementsWithCounters( tri, numIndexes );
		} else {
			// "preload" the stencil buffer with the number of volumes
			// that get clipped by the near or far clip plane
			qglStencilOp( GL_KEEP, tr.stencilDecr, tr.stencilDecr );
			GL_Cull( CT_FRONT_SIDED );
			RB_DrawShadowElementsWithCounters( tri, numIndexes );
	
			qglStencilOp( GL_KEEP, tr.stencilIncr, tr.stencilIncr );
			GL_Cull( CT_BACK_SIDED );
			RB_DrawShadowElementsWithCounters( tri, numIndexes );
		}
	} else {
		// traditional depth-pass stencil shadows
		if( r_useTwoSidedStencil.GetBool() && glConfig.twoSidedStencilAvailable ) {
			qglStencilOpSeparate( backEnd.viewDef->isMirror ? GL_FRONT : GL_BACK, GL_KEEP, GL_KEEP, tr.stencilIncr );
			qglStencilOpSeparate( backEnd.viewDef->isMirror ? GL_BACK : GL_FRONT, GL_KEEP, GL_KEEP, tr.stencilDecr );
			GL_Cull( CT_TWO_SIDED );
			RB_DrawShadowElementsWithCounters( tri, numIndexes );
		} else {	
			qglStencilOp( GL_KEEP, GL_KEEP, tr.stencilIncr );
			GL_Cull( CT_FRONT_SIDED );
			RB_DrawShadowElementsWithCounters( tri, numIndexes );
	
			qglStencilOp( GL_KEEP, GL_KEEP, tr.stencilDecr );
			GL_Cull( CT_BACK_SIDED );
			RB_DrawShadowElementsWithCounters( tri, numIndexes );
		}
	}
}

/*
=====================
RB_StencilShadowPass

Stencil test should already be enabled, and the stencil buffer should have
been set to 128 on any surfaces that might receive shadows
=====================
*/
void RB_StencilShadowPass( const drawSurf_t *drawSurfs ) {
	if ( !r_shadows.GetBool() ) {
		return;
	}

	if ( !drawSurfs ) {
		return;
	}

	RENDERLOG_PRINTF("---------- RB_StencilShadowPass ----------\n");

	globalImages->BindNull();
	qglDisableClientState( GL_TEXTURE_COORD_ARRAY );

	// for visualizing the shadows
	if ( r_showShadows.GetInteger() ) {
		renderProgManager.BindShader_ShadowDebug();

		if ( r_showShadows.GetInteger() == 2 ) {
			// draw filled in
			GL_State( GLS_DEPTHMASK | GLS_SRCBLEND_ONE | GLS_DSTBLEND_ONE | GLS_DEPTHFUNC_LESS  );
		} else {
			// draw as lines, filling the depth buffer
			GL_State( GLS_SRCBLEND_ONE | GLS_DSTBLEND_ZERO | GLS_POLYMODE_LINE | GLS_DEPTHFUNC_ALWAYS  );
		}
	} else {
		renderProgManager.BindShader_Shadow();

		// don't write to the color buffer, just the stencil buffer
		GL_State( GLS_DEPTHMASK | GLS_COLORMASK | GLS_ALPHAMASK | GLS_DEPTHFUNC_LESS );
	}

	if ( r_shadowPolygonFactor.GetFloat() || r_shadowPolygonOffset.GetFloat() ) {
		qglPolygonOffset( r_shadowPolygonFactor.GetFloat(), -r_shadowPolygonOffset.GetFloat() );
		qglEnable( GL_POLYGON_OFFSET_FILL );
	}

	qglStencilFunc( GL_ALWAYS, 1, 255 );

	if ( glConfig.depthBoundsTestAvailable && r_useDepthBoundsTest.GetBool() ) {
		qglEnable( GL_DEPTH_BOUNDS_TEST_EXT );
	}

	RB_RenderDrawSurfChainWithFunction( drawSurfs, RB_T_Shadow );

	GL_Cull( CT_FRONT_SIDED );

	if ( r_shadowPolygonFactor.GetFloat() || r_shadowPolygonOffset.GetFloat() ) {
		qglDisable( GL_POLYGON_OFFSET_FILL );
	}

	if ( glConfig.depthBoundsTestAvailable && r_useDepthBoundsTest.GetBool() ) {
		qglDisable( GL_DEPTH_BOUNDS_TEST_EXT );
	}

	qglEnableClientState( GL_TEXTURE_COORD_ARRAY );

	qglStencilFunc( GL_GEQUAL, 128, 255 );
	qglStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );

	renderProgManager.Unbind();
}



/*
=============================================================================================

BLEND LIGHT PROJECTION

=============================================================================================
*/

/*
=====================
RB_T_BlendLight

=====================
*/
static void RB_T_BlendLight( const drawSurf_t *surf ) {
	const srfTriangles_t *tri;

	tri = surf->geo;

	if ( backEnd.currentSpace != surf->space ) {
		idPlane	lightProject[4];
		int		i;

		for ( i = 0 ; i < 4 ; i++ ) {
			R_GlobalPlaneToLocal( surf->space->modelMatrix, backEnd.vLight->lightProject[i], lightProject[i] );
		}

		GL_SelectTexture( 0 );
		qglTexGenfv( GL_S, GL_OBJECT_PLANE, lightProject[0].ToFloatPtr() );
		qglTexGenfv( GL_T, GL_OBJECT_PLANE, lightProject[1].ToFloatPtr() );
		qglTexGenfv( GL_Q, GL_OBJECT_PLANE, lightProject[2].ToFloatPtr() );

		GL_SelectTexture( 1 );
		qglTexGenfv( GL_S, GL_OBJECT_PLANE, lightProject[3].ToFloatPtr() );
	}

	// this gets used for both blend lights and shadow draws
	if ( tri->ambientCache ) {
		idDrawVert	*ac = (idDrawVert *)vertexCache.Position( tri->ambientCache );
		qglVertexPointer( 3, GL_FLOAT, sizeof( idDrawVert ), ac->xyz.ToFloatPtr() );
	} else if ( tri->shadowCache ) {
		shadowCache_t	*sc = (shadowCache_t *)vertexCache.Position( tri->shadowCache );
		qglVertexPointer( 3, GL_FLOAT, sizeof( shadowCache_t ), sc->xyz.ToFloatPtr() );
	}

	RB_DrawElementsWithCounters( tri );
}


/*
=====================
RB_BlendLight

Dual texture together the falloff and projection texture with a blend
mode to the framebuffer, instead of interacting with the surface texture
=====================
*/
static void RB_BlendLight( const drawSurf_t *drawSurfs,  const drawSurf_t *drawSurfs2 ) {
	const idMaterial	*lightShader;
	const shaderStage_t	*stage;
	int					i;
	const float	*regs;

	if ( !drawSurfs ) {
		return;
	}
	if ( r_skipBlendLights.GetBool() ) {
		return;
	}
	RENDERLOG_PRINTF("---------- RB_BlendLight ----------\n");

	lightShader = backEnd.vLight->lightShader;
	regs = backEnd.vLight->shaderRegisters;

	// texture 1 will get the falloff texture
	GL_SelectTexture( 1 );
	qglDisableClientState( GL_TEXTURE_COORD_ARRAY );
	qglEnable( GL_TEXTURE_GEN_S );
	qglTexCoord2f( 0, 0.5 );
	backEnd.vLight->falloffImage->Bind();

	// texture 0 will get the projected texture
	GL_SelectTexture( 0 );
	qglDisableClientState( GL_TEXTURE_COORD_ARRAY );
	qglEnable( GL_TEXTURE_GEN_S );
	qglEnable( GL_TEXTURE_GEN_T );
	qglEnable( GL_TEXTURE_GEN_Q );

	for ( i = 0 ; i < lightShader->GetNumStages() ; i++ ) {
		stage = lightShader->GetStage(i);

		if ( !regs[ stage->conditionRegister ] ) {
			continue;
		}

		GL_State( GLS_DEPTHMASK | stage->drawStateBits | GLS_DEPTHFUNC_EQUAL );

		GL_SelectTexture( 0 );
		stage->texture.image->Bind();

		if ( stage->texture.hasMatrix ) {
			RB_LoadShaderTextureMatrix( regs, &stage->texture );
		}

		// get the modulate values from the light, including alpha, unlike normal lights
		backEnd.lightColor[0] = regs[ stage->color.registers[0] ];
		backEnd.lightColor[1] = regs[ stage->color.registers[1] ];
		backEnd.lightColor[2] = regs[ stage->color.registers[2] ];
		backEnd.lightColor[3] = regs[ stage->color.registers[3] ];
		qglColor4fv( backEnd.lightColor );

		RB_RenderDrawSurfChainWithFunction( drawSurfs, RB_T_BlendLight );
		RB_RenderDrawSurfChainWithFunction( drawSurfs2, RB_T_BlendLight );

		if ( stage->texture.hasMatrix ) {
			GL_SelectTexture( 0 );
			qglMatrixMode( GL_TEXTURE );
			qglLoadIdentity();
			qglMatrixMode( GL_MODELVIEW );
		}
	}

	GL_SelectTexture( 1 );
	qglDisable( GL_TEXTURE_GEN_S );
	globalImages->BindNull();

	GL_SelectTexture( 0 );
	qglDisable( GL_TEXTURE_GEN_S );
	qglDisable( GL_TEXTURE_GEN_T );
	qglDisable( GL_TEXTURE_GEN_Q );
}


//========================================================================

static idPlane	fogPlanes[4];

/*
=====================
RB_T_BasicFog

=====================
*/
static void RB_T_BasicFog( const drawSurf_t *surf ) {
	if ( backEnd.currentSpace != surf->space ) {
		idPlane	local;

		GL_SelectTexture( 0 );

		R_GlobalPlaneToLocal( surf->space->modelMatrix, fogPlanes[0], local );
		local[3] += 0.5;
		qglTexGenfv( GL_S, GL_OBJECT_PLANE, local.ToFloatPtr() );

//		R_GlobalPlaneToLocal( surf->space->modelMatrix, fogPlanes[1], local );
//		local[3] += 0.5;
local[0] = local[1] = local[2] = 0; local[3] = 0.5;
		qglTexGenfv( GL_T, GL_OBJECT_PLANE, local.ToFloatPtr() );

		GL_SelectTexture( 1 );

		// GL_S is constant per viewer
		R_GlobalPlaneToLocal( surf->space->modelMatrix, fogPlanes[2], local );
		local[3] += FOG_ENTER;
		qglTexGenfv( GL_T, GL_OBJECT_PLANE, local.ToFloatPtr() );

		R_GlobalPlaneToLocal( surf->space->modelMatrix, fogPlanes[3], local );
		qglTexGenfv( GL_S, GL_OBJECT_PLANE, local.ToFloatPtr() );
	}

	RB_T_RenderTriangleSurface( surf );
}



/*
==================
RB_FogPass
==================
*/
static void RB_FogPass( const drawSurf_t *drawSurfs,  const drawSurf_t *drawSurfs2 ) {
	const srfTriangles_t*frustumTris;
	drawSurf_t			ds;
	const idMaterial	*lightShader;
	const shaderStage_t	*stage;
	const float			*regs;

	RENDERLOG_PRINTF("---------- RB_FogPass ----------\n");

	// create a surface for the light frustom triangles, which are oriented drawn side out
	frustumTris = backEnd.vLight->frustumTris;

	// if we ran out of vertex cache memory, skip it
	if ( !frustumTris->ambientCache ) {
		return;
	}
	memset( &ds, 0, sizeof( ds ) );
	ds.space = &backEnd.viewDef->worldSpace;
	ds.geo = frustumTris;
	ds.scissorRect = backEnd.viewDef->scissor;

	// find the current color and density of the fog
	lightShader = backEnd.vLight->lightShader;
	regs = backEnd.vLight->shaderRegisters;
	// assume fog shaders have only a single stage
	stage = lightShader->GetStage(0);

	backEnd.lightColor[0] = regs[ stage->color.registers[0] ];
	backEnd.lightColor[1] = regs[ stage->color.registers[1] ];
	backEnd.lightColor[2] = regs[ stage->color.registers[2] ];
	backEnd.lightColor[3] = regs[ stage->color.registers[3] ];

	qglColor3fv( backEnd.lightColor );

	// calculate the falloff planes
	float	a;

	// if they left the default value on, set a fog distance of 500
	if ( backEnd.lightColor[3] <= 1.0 ) {
		a = -0.5f / DEFAULT_FOG_DISTANCE;
	} else {
		// otherwise, distance = alpha color
		a = -0.5f / backEnd.lightColor[3];
	}

	GL_State( GLS_DEPTHMASK | GLS_SRCBLEND_SRC_ALPHA | GLS_DSTBLEND_ONE_MINUS_SRC_ALPHA | GLS_DEPTHFUNC_EQUAL );

	// texture 0 is the falloff image
	GL_SelectTexture( 0 );
	globalImages->fogImage->Bind();
	//GL_Bind( tr.whiteImage );
	qglDisableClientState( GL_TEXTURE_COORD_ARRAY );
	qglEnable( GL_TEXTURE_GEN_S );
	qglEnable( GL_TEXTURE_GEN_T );
	qglTexCoord2f( 0.5f, 0.5f );		// make sure Q is set

	fogPlanes[0][0] = a * backEnd.viewDef->worldSpace.modelViewMatrix[2];
	fogPlanes[0][1] = a * backEnd.viewDef->worldSpace.modelViewMatrix[6];
	fogPlanes[0][2] = a * backEnd.viewDef->worldSpace.modelViewMatrix[10];
	fogPlanes[0][3] = a * backEnd.viewDef->worldSpace.modelViewMatrix[14];

	fogPlanes[1][0] = a * backEnd.viewDef->worldSpace.modelViewMatrix[0];
	fogPlanes[1][1] = a * backEnd.viewDef->worldSpace.modelViewMatrix[4];
	fogPlanes[1][2] = a * backEnd.viewDef->worldSpace.modelViewMatrix[8];
	fogPlanes[1][3] = a * backEnd.viewDef->worldSpace.modelViewMatrix[12];


	// texture 1 is the entering plane fade correction
	GL_SelectTexture( 1 );
	globalImages->fogEnterImage->Bind();
	qglDisableClientState( GL_TEXTURE_COORD_ARRAY );
	qglEnable( GL_TEXTURE_GEN_S );
	qglEnable( GL_TEXTURE_GEN_T );

	// T will get a texgen for the fade plane, which is always the "top" plane on unrotated lights
	fogPlanes[2][0] = 0.001f * backEnd.vLight->fogPlane[0];
	fogPlanes[2][1] = 0.001f * backEnd.vLight->fogPlane[1];
	fogPlanes[2][2] = 0.001f * backEnd.vLight->fogPlane[2];
	fogPlanes[2][3] = 0.001f * backEnd.vLight->fogPlane[3];

	// S is based on the view origin
	float s = backEnd.viewDef->renderView.vieworg * fogPlanes[2].Normal() + fogPlanes[2][3];

	fogPlanes[3][0] = 0;
	fogPlanes[3][1] = 0;
	fogPlanes[3][2] = 0;
	fogPlanes[3][3] = FOG_ENTER + s;

	qglTexCoord2f( FOG_ENTER + s, FOG_ENTER );


	// draw it
	RB_RenderDrawSurfChainWithFunction( drawSurfs, RB_T_BasicFog );
	RB_RenderDrawSurfChainWithFunction( drawSurfs2, RB_T_BasicFog );

	// the light frustum bounding planes aren't in the depth buffer, so use depthfunc_less instead
	// of depthfunc_equal
	GL_State( GLS_DEPTHMASK | GLS_SRCBLEND_SRC_ALPHA | GLS_DSTBLEND_ONE_MINUS_SRC_ALPHA | GLS_DEPTHFUNC_LESS );
	GL_Cull( CT_BACK_SIDED );
	RB_RenderDrawSurfChainWithFunction( &ds, RB_T_BasicFog );
	GL_Cull( CT_FRONT_SIDED );

	GL_SelectTexture( 1 );
	qglDisable( GL_TEXTURE_GEN_S );
	qglDisable( GL_TEXTURE_GEN_T );
	globalImages->BindNull();

	GL_SelectTexture( 0 );
	qglDisable( GL_TEXTURE_GEN_S );
	qglDisable( GL_TEXTURE_GEN_T );
}


/*
==================
RB_STD_FogAllLights
==================
*/
void RB_STD_FogAllLights( void ) {
	viewLight_t	*vLight;

	if ( r_skipFogLights.GetBool() || r_showOverDraw.GetInteger() != 0 
		 || backEnd.viewDef->isXraySubview /* dont fog in xray mode*/
		 ) {
		return;
	}

	RENDERLOG_PRINTF("---------- RB_STD_FogAllLights ----------\n");

	qglDisable( GL_STENCIL_TEST );

	for ( vLight = backEnd.viewDef->viewLights ; vLight ; vLight = vLight->next ) {
		backEnd.vLight = vLight;

		if ( !vLight->lightShader->IsFogLight() && !vLight->lightShader->IsBlendLight() ) {
			continue;
		}

#if 0 // _D3XP disabled that
		if ( r_ignore.GetInteger() ) {
			// we use the stencil buffer to guarantee that no pixels will be
			// double fogged, which happens in some areas that are thousands of
			// units from the origin
			backEnd.currentScissor = vLight->scissorRect;
			if ( r_useScissor.GetBool() ) {
				qglScissor( backEnd.viewDef->viewport.x1 + backEnd.currentScissor.x1, 
					backEnd.viewDef->viewport.y1 + backEnd.currentScissor.y1,
					backEnd.currentScissor.x2 + 1 - backEnd.currentScissor.x1,
					backEnd.currentScissor.y2 + 1 - backEnd.currentScissor.y1 );
			}
			qglClear( GL_STENCIL_BUFFER_BIT );

			qglEnable( GL_STENCIL_TEST );

			// only pass on the cleared stencil values
			qglStencilFunc( GL_EQUAL, 128, 255 );

			// when we pass the stencil test and depth test and are going to draw,
			// increment the stencil buffer so we don't ever draw on that pixel again
			qglStencilOp( GL_KEEP, GL_KEEP, GL_INCR );
		}
#endif

		if ( vLight->lightShader->IsFogLight() ) {
			RB_FogPass( vLight->globalInteractions, vLight->localInteractions );
		} else if ( vLight->lightShader->IsBlendLight() ) {
			RB_BlendLight( vLight->globalInteractions, vLight->localInteractions );
		}
		qglDisable( GL_STENCIL_TEST );
	}

	qglEnable( GL_STENCIL_TEST );
}


/*
=========================================================================================

GENERAL INTERACTION RENDERING

=========================================================================================
*/


/*
====================
GL_SelectTextureNoClient
====================
*/
static void GL_SelectTextureNoClient(int unit) {
	backEnd.glState.currenttmu = unit;
	qglActiveTextureARB(GL_TEXTURE0_ARB + unit);
	RENDERLOG_PRINTF("glActiveTextureARB( %i )\n", unit);
}

/*
==================
RB_STD_DrawInteraction
==================
*/
void RB_STD_DrawInteraction(const drawInteraction_t *din) {
	// load all the vertex program parameters
	renderProgManager.SetRenderParm( RENDERPARM_LOCALLIGHTORIGIN, din->localLightOrigin.ToFloatPtr() );
	renderProgManager.SetRenderParm( RENDERPARM_LOCALVIEWORIGIN, din->localViewOrigin.ToFloatPtr() );

	renderProgManager.SetRenderParm( RENDERPARM_LIGHTPROJECTION_S, din->lightProjection[0].ToFloatPtr() );
	renderProgManager.SetRenderParm( RENDERPARM_LIGHTPROJECTION_T, din->lightProjection[1].ToFloatPtr() );
	renderProgManager.SetRenderParm( RENDERPARM_LIGHTPROJECTION_Q, din->lightProjection[2].ToFloatPtr() );
	renderProgManager.SetRenderParm( RENDERPARM_LIGHTFALLOFF_S, din->lightProjection[3].ToFloatPtr() );

	renderProgManager.SetRenderParm( RENDERPARM_BUMPMATRIX_S, din->bumpMatrix[0].ToFloatPtr() );
	renderProgManager.SetRenderParm( RENDERPARM_BUMPMATRIX_T, din->bumpMatrix[1].ToFloatPtr() );
	renderProgManager.SetRenderParm( RENDERPARM_DIFFUSEMATRIX_S, din->diffuseMatrix[0].ToFloatPtr() );
	renderProgManager.SetRenderParm( RENDERPARM_DIFFUSEMATRIX_T, din->diffuseMatrix[1].ToFloatPtr() );
	renderProgManager.SetRenderParm( RENDERPARM_SPECULARMATRIX_S, din->specularMatrix[0].ToFloatPtr() );
	renderProgManager.SetRenderParm( RENDERPARM_SPECULARMATRIX_T, din->specularMatrix[1].ToFloatPtr() );

	// set the vertex colors
	RB_SetVertexColorParms( din->vertexColor );

	// set the textures

	// texture 0 will be the per-surface bump map
	GL_SelectTextureNoClient(0);
	din->bumpImage->Bind();

	// texture 1 will be the light falloff texture
	GL_SelectTextureNoClient(1);
	din->lightFalloffImage->Bind();

	// texture 2 will be the light projection texture
	GL_SelectTextureNoClient(2);
	din->lightImage->Bind();

	// texture 3 is the per-surface diffuse map
	GL_SelectTextureNoClient(3);
	din->diffuseImage->Bind();

	// texture 4 is the per-surface specular map
	GL_SelectTextureNoClient(4);
	din->specularImage->Bind();

	// draw it
	RB_DrawElementsWithCounters(din->surf->geo);
}


/*
=============
RB_STD_CreateDrawInteractions
=============
*/
void RB_STD_CreateDrawInteractions(const drawSurf_t *surf) {
	if (!surf) {
		return;
	}

	// perform setup here that will be constant for all interactions
	GL_State(GLS_SRCBLEND_ONE | GLS_DSTBLEND_ONE | GLS_DEPTHMASK | backEnd.depthFunc);

	// select the render prog
	if ( surf->material->IsAmbientLight() ) {
		renderProgManager.BindShader_InteractionAmbient();
	} else {
		renderProgManager.BindShader_Interaction();
	}

	// enable the vertex arrays
	qglEnableVertexAttribArrayARB(8);
	qglEnableVertexAttribArrayARB(9);
	qglEnableVertexAttribArrayARB(10);
	qglEnableVertexAttribArrayARB(11);
	qglEnableClientState(GL_COLOR_ARRAY);

	for (; surf; surf = surf->nextOnLight) {
		// perform setup here that will not change over multiple interaction passes

		// set the modelview matrix for the viewer
		float mat[16];
		myGlMultMatrix( surf->space->modelViewMatrix, backEnd.viewDef->projectionMatrix, mat );
		renderProgManager.SetRenderParms( RENDERPARM_MVPMATRIX_X, mat, 4 );

		// set the vertex pointers
		idDrawVert	*ac = (idDrawVert *)vertexCache.Position(surf->geo->ambientCache);
		qglColorPointer(4, GL_UNSIGNED_BYTE, sizeof(idDrawVert), ac->color);
		qglVertexAttribPointerARB(11, 3, GL_FLOAT, false, sizeof(idDrawVert), ac->normal.ToFloatPtr());
		qglVertexAttribPointerARB(10, 3, GL_FLOAT, false, sizeof(idDrawVert), ac->tangents[1].ToFloatPtr());
		qglVertexAttribPointerARB(9, 3, GL_FLOAT, false, sizeof(idDrawVert), ac->tangents[0].ToFloatPtr());
		qglVertexAttribPointerARB(8, 2, GL_FLOAT, false, sizeof(idDrawVert), ac->st.ToFloatPtr());
		qglVertexPointer(3, GL_FLOAT, sizeof(idDrawVert), ac->xyz.ToFloatPtr());

		// this may cause RB_STD_DrawInteraction to be executed multiple
		// times with different colors and images if the surface or light have multiple layers
		RB_CreateSingleDrawInteractions(surf, RB_STD_DrawInteraction);
	}

	qglDisableVertexAttribArrayARB(8);
	qglDisableVertexAttribArrayARB(9);
	qglDisableVertexAttribArrayARB(10);
	qglDisableVertexAttribArrayARB(11);
	qglDisableClientState(GL_COLOR_ARRAY);

	// disable features
	GL_SelectTextureNoClient(4);
	globalImages->BindNull();

	GL_SelectTextureNoClient(3);
	globalImages->BindNull();

	GL_SelectTextureNoClient(2);
	globalImages->BindNull();

	GL_SelectTextureNoClient(1);
	globalImages->BindNull();

	backEnd.glState.currenttmu = -1;
	GL_SelectTexture(0);

	renderProgManager.Unbind();
}


/*
==================
RB_STD_DrawInteractions
==================
*/
void RB_STD_DrawInteractions(void) {
	viewLight_t		*vLight;
	const idMaterial	*lightShader;

	renderLog.OpenMainBlock( MRB_DRAW_INTERACTIONS );
	renderLog.OpenBlock( "RB_DrawInteractions" );

	GL_SelectTexture(0);
	qglDisableClientState(GL_TEXTURE_COORD_ARRAY);

	//
	// for each light, perform adding and shadowing
	//
	for (vLight = backEnd.viewDef->viewLights; vLight; vLight = vLight->next) {
		backEnd.vLight = vLight;

		// do fogging later
		if (vLight->lightShader->IsFogLight()) {
			continue;
		}
		if (vLight->lightShader->IsBlendLight()) {
			continue;
		}

		if (!vLight->localInteractions && !vLight->globalInteractions && !vLight->translucentInteractions) {
			continue;
		}

		lightShader = vLight->lightShader;

		// clear the stencil buffer if needed
		if (vLight->globalShadows || vLight->localShadows) {
			backEnd.currentScissor = vLight->scissorRect;
			if (r_useScissor.GetBool()) {
				qglScissor(backEnd.viewDef->viewport.x1 + backEnd.currentScissor.x1,
					backEnd.viewDef->viewport.y1 + backEnd.currentScissor.y1,
					backEnd.currentScissor.x2 + 1 - backEnd.currentScissor.x1,
					backEnd.currentScissor.y2 + 1 - backEnd.currentScissor.y1);
			}
			qglClear(GL_STENCIL_BUFFER_BIT);
		} else {
			// no shadows, so no need to read or write the stencil buffer
			// we might in theory want to use GL_ALWAYS instead of disabling
			// completely, to satisfy the invarience rules
			qglStencilFunc(GL_ALWAYS, 128, 255);
		}

		if ( vLight->globalShadows != NULL ) {
			renderLog.OpenBlock( "Global Light Shadows" );
			RB_StencilShadowPass( vLight->globalShadows );
			renderLog.CloseBlock();
		}

		if ( vLight->localInteractions != NULL ) {
			renderLog.OpenBlock( "Local Light Interactions" );
			RB_STD_CreateDrawInteractions( vLight->localInteractions );
			renderLog.CloseBlock();
		}

		if ( vLight->localShadows != NULL ) {
			renderLog.OpenBlock( "Local Light Shadows" );
			RB_StencilShadowPass( vLight->localShadows );
			renderLog.CloseBlock();
		}

		if ( vLight->globalInteractions != NULL ) {
			renderLog.OpenBlock( "Global Light Interactions" );
			RB_STD_CreateDrawInteractions( vLight->globalInteractions );
			renderLog.CloseBlock();
		}
		
		if ( vLight->translucentInteractions != NULL && !r_skipTranslucent.GetBool() ) {
			renderLog.OpenBlock( "Translucent Interactions" );

			// disable stencil shadow test
			qglStencilFunc( GL_ALWAYS, 128, 255 );

			// Disable the depth bounds test because translucent surfaces don't work with
			// the depth bounds tests since they did not write depth during the depth pass.
			backEnd.depthFunc = GLS_DEPTHFUNC_LESS;

			// The depth buffer wasn't filled in for translucent surfaces, so they
			// can never be constrained to perforated surfaces with the depthfunc equal.

			// Translucent surfaces do not receive shadows. This is a case where a
			// shadow buffer solution would work but stencil shadows do not because
			// stencil shadows only affect surfaces that contribute to the view depth
			// buffer and translucent surfaces do not contribute to the view depth buffer.
			RB_STD_CreateDrawInteractions( vLight->translucentInteractions );

			backEnd.depthFunc = GLS_DEPTHFUNC_EQUAL;

			renderLog.CloseBlock();
		}
	}

	// disable stencil shadow test
	qglStencilFunc(GL_ALWAYS, 128, 255);

	GL_SelectTexture(0);
	qglEnableClientState(GL_TEXTURE_COORD_ARRAY);

	renderLog.CloseBlock();
	renderLog.CloseMainBlock();
}

//=========================================================================================

/*
=============
RB_STD_DrawView
=============
*/
void RB_STD_DrawView( void ) {
	drawSurf_t	 **drawSurfs;
	int			numDrawSurfs;

	RENDERLOG_PRINTF("---------- RB_STD_DrawView ----------\n");

	backEnd.depthFunc = GLS_DEPTHFUNC_EQUAL;

	drawSurfs = (drawSurf_t **)&backEnd.viewDef->drawSurfs[0];
	numDrawSurfs = backEnd.viewDef->numDrawSurfs;

	// clear the z buffer, set the projection matrix, etc
	RB_BeginDrawingView();

	// fill the depth buffer and clear color buffer to black except on
	// subviews
	RB_STD_FillDepthBuffer( drawSurfs, numDrawSurfs );

	// main light renderer
	RB_STD_DrawInteractions();

	// disable stencil shadow test
	qglStencilFunc( GL_ALWAYS, 128, 255 );
	
	// now draw any non-light dependent shading passes
	int	processed = RB_STD_DrawShaderPasses( drawSurfs, numDrawSurfs );

	// fob and blend lights
	RB_STD_FogAllLights();

	// now draw any post-processing effects using _currentRender
	if ( processed < numDrawSurfs ) {
		RB_STD_DrawShaderPasses( drawSurfs+processed, numDrawSurfs-processed );
	}

	RB_RenderDebugTools( drawSurfs, numDrawSurfs );
}

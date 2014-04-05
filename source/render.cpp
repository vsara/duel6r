/*
* Copyright (c) 2006, Ondrej Danek (www.ondrej-danek.net)
* All rights reserved.
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of Ondrej Danek nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
* GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "project.h"

static void RENDER_Blocks (int start, int blocks)
{
    myUINT      *ta = d6World.Anm.TexGlNum, t;
    d6FACE      *df = d6World.Face;
    int         i, v, c = 0;

    t = ta[df[start].NowTex];
    v = start << 2;
    blocks += start;

    for (i = start; i < blocks; i++)
        if (ta[df[i].NowTex] != t)
        {
            glBindTexture (GL_TEXTURE_2D, t);
            glDrawArrays (GL_QUADS, v, c);
            t = ta[df[i].NowTex];
            v += c;
            c = 4;
        }
        else
            c += 4;

    glBindTexture (GL_TEXTURE_2D, t);
    glDrawArrays (GL_QUADS, v, c);
}

static void RENDER_Water (void)
{
    if (!d6World.Waters)
        return;

    glDisable (GL_CULL_FACE);
    glDepthMask (GL_FALSE);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);

    RENDER_Blocks (d6World.Blocks + d6World.Sprites, d6World.Waters);

    glDisable (GL_BLEND);
    glDepthMask (GL_TRUE);
    glEnable (GL_CULL_FACE);
}

static void RENDER_Sprites (void)
{
    if (!d6World.Sprites)
        return;

    glEnable (GL_ALPHA_TEST);
    glDisable (GL_CULL_FACE);

    RENDER_Blocks (d6World.Blocks, d6World.Sprites);

    glDisable (GL_ALPHA_TEST);
    glEnable (GL_CULL_FACE);
}

static void RENDER_MoveAnm (void)
{
    d6FACE  *f = d6World.Face;
    int     i;

    if ((d6World.Anm.Wait += g_app.frame_interval) > D6_ANM_SPEED)
    {
        d6World.Anm.Wait = 0;
        for (i = 0; i < d6World.Faces; i++)
            if (++f[i].NowTex > f[i].MaxTex)
                f[i].NowTex = f[i].MinTex;
    }
}

static void RENDER_Background (void)
{
    glEnable (GL_TEXTURE_2D);
    glBindTexture (GL_TEXTURE_2D, d6BackTex);

    glBegin (GL_QUADS);
        glTexCoord2f (0, 0); glVertex2i (0, g_vid.cl_height);
        glTexCoord2f (1, 0); glVertex2i (g_vid.cl_width, g_vid.cl_height);
        glTexCoord2f (1, 1); glVertex2i (g_vid.cl_width, 0);
        glTexCoord2f (0, 1); glVertex2i (0, 0);
    glEnd ();

    glDisable (GL_TEXTURE_2D);
}

static void RENDER_PlayerStatus (d6PLSTATE_s *s)
{
    int     al, ll, alpha = 180;

    if (s->Flags & D6_FLAG_DEAD)
        return;

    al = int ((s->Air * 125) / D6_MAX_AIR);
    ll = int ((s->Life * 125) / D6_MAX_LIFE);

    glEnable (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBegin (GL_QUADS);
        glColor4ub (255, 255, 0, alpha);
        glVertex2i (s->IBP[0], s->IBP[1] + 2);
        glVertex2i (s->IBP[0] + 151, s->IBP[1] + 2);
        glVertex2i (s->IBP[0] + 151, s->IBP[1] - 25);
        glVertex2i (s->IBP[0], s->IBP[1] - 25);

        glColor4ub (200, 200, 0, alpha);
        glVertex2i (s->IBP[0] + 23, s->IBP[1] + 1);
        glVertex2i (s->IBP[0] + 130, s->IBP[1] + 1);
        glVertex2i (s->IBP[0] + 130, s->IBP[1] - 13);
        glVertex2i (s->IBP[0] + 23, s->IBP[1] - 13);

        glColor4ub (255, 0, 0, alpha);
        glVertex2i (s->IBP[0] + 5, s->IBP[1] - 14);
        glVertex2i (s->IBP[0] + 5 + ll, s->IBP[1] - 14);
        glVertex2i (s->IBP[0] + 5 + ll, s->IBP[1] - 18);
        glVertex2i (s->IBP[0] + 5, s->IBP[1] - 18);

        glColor4ub (0, 0, 255, alpha);
        glVertex2i (s->IBP[0] + 5, s->IBP[1] - 19);
        glVertex2i (s->IBP[0] + 5 + al, s->IBP[1] - 19);
        glVertex2i (s->IBP[0] + 5 + al, s->IBP[1] - 23);
        glVertex2i (s->IBP[0] + 5, s->IBP[1] - 23);
    glEnd ();

    CO_FontPrintf (s->IBP[0] + 5, s->IBP[1] - 13, "%d", s->Ammo);
    CO_FontPrintf (s->IBP[0] + 76 - 4 * strlen (s->PH->Name), s->IBP[1] - 13, s->PH->Name);

    if (s->Bonus)
    {
        glEnable (GL_TEXTURE_2D);
        glEnable (GL_ALPHA_TEST);
        glBindTexture (GL_TEXTURE_2D, d6World.Anm.TexGlNum[s->Bonus]);
        glColor3ub (255, 255, 255);
        glBegin (GL_QUADS);
            glTexCoord2f (0.3f, 0.3f); glVertex2i (s->IBP[0] + 133, s->IBP[1] - 3);
            glTexCoord2f (0.7f, 0.3f); glVertex2i (s->IBP[0] + 148, s->IBP[1] - 3);
            glTexCoord2f (0.7f, 0.7f); glVertex2i (s->IBP[0] + 148, s->IBP[1] - 18);
            glTexCoord2f (0.3f, 0.7f); glVertex2i (s->IBP[0] + 133, s->IBP[1] - 18);
        glEnd ();
        glDisable (GL_TEXTURE_2D);
        glDisable (GL_ALPHA_TEST);
    }

    glDisable (GL_BLEND);
}

static void RENDER_PlayersInfo (void)
{
    int     i, x, y;

    CO_FontColor (0, 0, 255);

    for (i = 0; i < d6Playing; i++)
        RENDER_PlayerStatus (&d6Player[i]->State);

    if (d6ShowFps)
    {
        x = g_vid.cl_width - 80;
        y = g_vid.cl_height - 20;
        i = (int) g_app.fps;
        if (i < 10)
            i = 1;
        else if (i < 100)
            i = 2;
        else if (i < 1000)
            i = 3;
        else
            i = 4;
        i = ((6 + i) << 3) + 2;

        glBegin (GL_QUADS);
            glColor3f (0.0f, 0.0f, 0.0f);
            glVertex2i (x - 1, y + 17);
            glVertex2i (x + i, y + 17);
            glVertex2i (x + i, y - 1);
            glVertex2i (x - 1, y - 1);
        glEnd ();

        CO_FontColor (255, 255, 255);
        CO_FontPrintf (x, y, "FPS - %d", (int) g_app.fps);
    }

    INFO_DrawAll ();
}

static void RENDER_InvulRing (d6PLSTATE_s *s)
{
    float   x, y, X, Y;
    int     p, uh, u;

    x = s->X + 0.5f;
    y = s->Y - 0.5f;
    p = (int(s->BD) / 2) % 360;

    glColor3ub (255, 0, 0);
    glDisable (GL_TEXTURE_2D);
    glPointSize (2.0f);
    glBegin (GL_POINTS);

    for (uh = p; uh < 360 + p; uh += 15)
    {
        u = uh % 360;
        X = x + 0.7f * d6Sin[90 + u];
        Y = y + 0.7f * d6Sin[u];
        glVertex3f (X, Y, 0.5f);
    }

    glEnd ();
    glEnable (GL_TEXTURE_2D);
    glPointSize (1.0f);
    glColor3ub (255, 255, 255);
}

static void RENDER_Invuls (d6PLSTATE_s *s)
{
    int     i;

    if (d6ZoomMode == D6_ZM_FULL)
    {
        for (i = 0; i < d6Playing; i++)
            if (d6Player[i]->State.Bonus == D6_BONUS_INVUL)
                RENDER_InvulRing (&d6Player[i]->State);
    }
    else
        if (s->Bonus == D6_BONUS_INVUL)
            RENDER_InvulRing (s);
}

static void RENDER_SplitBox (d6VIEW_s *view)
{
    glViewport (view->X - 2, view->Y - 2, view->Width + 4, view->Height + 4);
    glColor3f (1, 0, 0);
    glBegin (GL_QUADS);
        glVertex2i (0, 0);
        glVertex2i (0, g_vid.cl_height);
        glVertex2i (g_vid.cl_width, g_vid.cl_height);
        glVertex2i (g_vid.cl_width, 0);
    glEnd ();
    glColor3f (1, 1, 1);
    glViewport (view->X, view->Y, view->Width, view->Height);
}

static void RENDER_View (d6PLAYER_c *p)
{
    glLoadIdentity ();
    p->Camera->look ();

    if (d6Wireframe)
        glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);

    if ((p->State.Flags & D6_FLAG_DEAD) && d6ZoomMode == D6_ZM_SCROLL)
        glColor3f (1.0f, 0.5f, 0.5f);

    RENDER_Blocks (0, d6World.Blocks);
    RENDER_Sprites ();
    ELEV_DrawAll ();
    ANM_DrawAll ();
    BONUS_DrawAll ();
    RENDER_Invuls (&p->State);
    RENDER_Water ();
    EXPL_DrawAll ();

    if (d6Wireframe)
        glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
    glColor3f (1, 1, 1);
}

void RENDER_MoveScene (void)
{
    CO_InpUpdate ();
    PLAYER_UpdateAll ();
    RENDER_MoveAnm ();
    ANM_MoveAll ();
    WATER_Move ();
    WPN_MoveShots ();
    EXPL_MoveAll ();
    ELEV_MoveAll ();
    INFO_MoveAll ();
    BONUS_AddNew ();

    // Ochrana pred nekolikanasobnym zmacknutim klavesy
    d6KeyWait -= g_app.frame_interval;
    if (d6KeyWait < 0)
        d6KeyWait = 0;
}

void RENDER_DrawScene (void)
{
    int     i;

    if (d6Wireframe || d6ZoomMode != D6_ZM_FULL)
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    else
        glClear (GL_DEPTH_BUFFER_BIT);

	if (d6Winner < 0)
	{
		glColor3f(1, 1, 1);
	}
	else
	{
		float overlay = d6GameOverWait / D6_GAME_OVER_WAIT;
		glColor3f(1, overlay, overlay);
	}

    if (d6ZoomMode == D6_ZM_SCROLL)
        for (i = 0; i < d6Playing; i++)
        {
            RENDER_SplitBox (&d6Player[i]->View);

            if ((d6Player[i]->State.Flags & D6_FLAG_DEAD) && d6ZoomMode == D6_ZM_SCROLL)
                glColor3f (1.0f, 0.5f, 0.5f);

            RENDER_Background ();
            glColor3f (1, 1, 1);
        }
    else
    {
        D6_SetView (&d6Player[0]->View);
        RENDER_Background ();
    }

    D6_SetGLMode (D6_GL_PERSPECTIVE);

    if (d6ZoomMode == D6_ZM_SCROLL)
        for (i = 0; i < d6Playing; i++)
        {
            D6_SetView (&d6Player[i]->View);
            RENDER_View (d6Player[i]);
        }
    else
        RENDER_View (d6Player[0]);

    D6_SetGLMode (D6_GL_ORTHO);
    D6_SetView (0, 0, g_vid.cl_width, g_vid.cl_height);
    RENDER_PlayersInfo ();
}
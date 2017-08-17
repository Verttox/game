#pragma once

#include "cbase.h"
#include "mom_ghost_base.h"
#include "mom_ghostdefs.h"
#include "utlstack.h"
#include "utlqueue.h"

class CMomentumOnlineGhostEntity : public CMomentumGhostBaseEntity, public CGameEventListener
{
    DECLARE_CLASS(CMomentumOnlineGhostEntity, CMomentumGhostBaseEntity)
    DECLARE_DATADESC();
    DECLARE_SERVERCLASS();

public:
    CMomentumOnlineGhostEntity();
    ~CMomentumOnlineGhostEntity();

    void SetCurrentNetFrame(ghostNetFrame_t newFrame);
    //ghostNetFrame_t GetCurrentNetFrame() const { return m_currentFrame; }
    void SetGhostSteamID(CSteamID steamID)
    {
        m_GhostSteamID = steamID;
        m_uiAccountID = m_GhostSteamID.ConvertToUint64();
    }
    CSteamID GetGhostSteamID() const { return m_GhostSteamID; }
    void SetGhostName(const char* pGhostName)
    {
        Q_strncpy(m_pszGhostName.GetForModify(), pGhostName, MAX_PLAYER_NAME_LENGTH);
    }

    void SetGhostAppearance(LobbyGhostAppearance_t app);

    bool IsOnlineGhost() const OVERRIDE { return true; }

    void Spawn() OVERRIDE;
    void HandleGhost() OVERRIDE;
    void HandleGhostFirstPerson() OVERRIDE;
    void UpdateStats(const Vector &ghostVel) OVERRIDE; // for hud display..
    void HideGhost()
    {
        // don't render the model when we're in first person mode
        if (GetRenderMode() != kRenderNone)
        {
            SetRenderMode(kRenderNone);
            AddEffects(EF_NOSHADOW);
        }
    }
    void UnHideGhost()
    {
        if (GetRenderMode() != kRenderTransColor)
        {
            SetRenderMode(kRenderTransColor);
            RemoveEffects(EF_NOSHADOW);
        }
    }
    CNetworkVar(int, m_nGhostButtons);
    CNetworkVar(uint32, m_uiAccountID);
    CNetworkString(m_pszGhostName, MAX_PLAYER_NAME_LENGTH);

protected:
    void Think(void) OVERRIDE;
    void Precache(void) OVERRIDE;
    void FireGameEvent(IGameEvent *pEvent) OVERRIDE;

private:
    CUtlQueue<ReceivedFrame_t*> m_vecFrames;
    ReceivedFrame_t* m_pCurrentFrame;
    ReceivedFrame_t* m_pNextFrame;

    CSteamID m_GhostSteamID;
    LobbyGhostAppearance_t m_CurrentAppearance;
};

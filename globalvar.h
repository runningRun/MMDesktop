#ifndef GLOBALVAR_H
#define GLOBALVAR_H

#include <QString>

struct UserInfo
{
    QString thisDN;
    QString via;
    QString sessionid;
    QString content;
};

enum MSGTYPE
{
    TEXT = 1,
    PICTURE = 2,
    VOICE = 3,
    VIDEO = 4
};

enum EventID {
    EventAgentStattusChanged = 0,
    EventSessionStatusChanged = 1,
    EventCallStatusChanged = 2,
    EventChatMessage = 3
};

enum AGENT_STATUS
{
    STATE_AGENT_LOGOUT = 0,
    STATE_AGENT_LOGGEDIN = 1,
    STATE_AGENT_READY = 2,
    STATE_AGENT_NOTREADY = 3,
    STATE_AGENT_ACW = 4,
    STATE_AGENT_NOTREADY_FOR_NEXTCALL = 5,
    STATE_AGENT_HAS_SESSIONS = 6,
    STATE_AGENT_NO_SESSIONS = 7,
};

enum SESSION_STATUS
{
    STATE_SESSION_IVR = 0,
    STATE_SESSION_QUEUED = 1,
    STATE_SESSION_DIVERTED = 2,
    STATE_SESSION_RINGING = 3,
    STATE_SESSION_ESTABLISHED = 4,
    STATE_SESSION_RELEASED = 5,
    STATE_SESSION_HELD = 6,
};

#endif // GLOBALVAR_H


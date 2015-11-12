#include "agentinfo.h"

AgentInfo::AgentInfo()
{

}

QString AgentInfo::agentGroupID = "";
QString AgentInfo::agentID = "";
QString AgentInfo::agentName = "";
QString AgentInfo::agentThisDN = "";
AgentInfo* AgentInfo::m_pAgentInfo = NULL;

AgentInfo* AgentInfo::instance()
{
    if (NULL == m_pAgentInfo)
    {
        m_pAgentInfo = new AgentInfo();
    }
    return m_pAgentInfo;
}


AgentInfo::~AgentInfo()
{

}


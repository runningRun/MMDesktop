#ifndef AGENTINFO_H
#define AGENTINFO_H

#include<QString>

class AgentInfo
{
public:
    AgentInfo();
    ~AgentInfo();
    static QString agentName;
    static QString agentID;
    static QString agentGroupID;
    static QString agentThisDN;

    static AgentInfo* instance();
private:
    static AgentInfo* m_pAgentInfo;
};

#endif // AGENTINFO_H

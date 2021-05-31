#ifndef ITANDROIDS_SOCCER3D_CPP_KICKKEYFRAMEEVALUATION_AGENTTEST_H
#define ITANDROIDS_SOCCER3D_CPP_KICKKEYFRAMEEVALUATION_AGENTTEST_H

#include "../../../test_lib/base/BaseAgentTest.h"
#include "../../../../core/decision_making/role/AttackingRole.h"

class KickKeyframeEvaluation_AgentTest : public BaseAgentTest{
    public:
        void setup();
        void runStep();
    private:
        std::ofstream output;
        decision_making::role::AttackingRole *role;
        double time = 0;
        double maxHeight = 0;
        double numTests = 0;
};

#endif //ITANDROIDS_SOCCER3D_CPP_KICKKEYFRAMEEVALUATION_AGENTTEST_H

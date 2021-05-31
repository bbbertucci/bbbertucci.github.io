#ifndef ITANDROIDS_SOCCER3D_CPP_KICKKEYFRAMEOPTIMIZATION_AGENTTEST_H
#define ITANDROIDS_SOCCER3D_CPP_KICKKEYFRAMEOPTIMIZATION_AGENTTEST_H

#include "../../../test_lib/base/BaseAgentTest.h"
#include "../../../../core/decision_making/role/AttackingRole.h"
#include <math.h>

class KickKeyframeOptimization_AgentTest : public BaseAgentTest {
    public:
    void runStep();
    void setup();
    void reset();
    double calculateReward();
    private:
    const double deltaAngle = 45*M_PI/180;
    const double TIME_LIMIT = 10;
    const double EPSILON = 0.001;
    decision_making::role::AttackingRole *role;
    int iterator = 0;
    double time = 0;
    bool startedKick = false;
    double maxHeight = 0;
    double cumulativeReward = 0;
    Vector3<double> oldBallPos;
    Vector3<double> ballOrigin;
    bool isFallen = false;
    bool ballMoved = false;
};

#endif //ITANDROIDS_SOCCER3D_CPP_KICKKEYFRAMEOPTIMIZATION_AGENTTEST_H
//
// created by bruno on 08/15/2020
//

#include "KickKeyframeEvaluation_AgentTest.h"


void KickKeyframeEvaluation_AgentTest::setup() {
    this->output = std::ofstream("evaluation.txt");
    this->output << "dist_x, dist_y, max_height" << endl;
    role = new decision_making::role::AttackingRole();
    wiz.setPlayMode(representations::RawPlayMode::PLAY_ON);
    wiz.setAgentPositionAndDirection(10, representations::PlaySide::LEFT, Vector3<double>(-1,0,0.5));
}

void KickKeyframeEvaluation_AgentTest::runStep() {
    if(not decisionMaking.decideFall(modeling)) {
        role->execute(modeling);
        decisionMaking.lookRequest = role->getLookRequest();
        decisionMaking.movementRequest = role->getMovementRequest();
        decisionMaking.passRequest = role->getPassRequest();
        decisionMaking.sayMessage = role->getSayMessage();
    }

    if(this->time > 15) {
        Vector3<double> ballPos = wiz.getBallTranslation();
        this->output << to_string(ballPos.x) + "," + to_string(ballPos.y) + "," + to_string(this->maxHeight) << endl;
        this->numTests++;
        this->time = 0;
        this->maxHeight = 0;

        if(numTests == 20) {
            this->output.close();
            this->isDone = true;
        }

        wiz.setAgentPositionAndDirection(10, representations::PlaySide::LEFT, Vector3<double>(-1,0,0.5));
        wiz.setBallPositionAndVelocity(Vector3<double>(0,0,0.3), Vector3<double>(0,0,0));

    } else {
        this->time += 0.02;
        double height = wiz.getBallTranslation().z;
        if(height > this->maxHeight) {
            this->maxHeight = height;
        }
    }
}

int main() {
    KickKeyframeEvaluation_AgentTest agent;
    agent.testLoop();
}
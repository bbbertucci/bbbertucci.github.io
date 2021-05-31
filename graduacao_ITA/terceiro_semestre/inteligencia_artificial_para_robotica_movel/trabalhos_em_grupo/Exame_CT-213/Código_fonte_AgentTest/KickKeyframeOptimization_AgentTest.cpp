//
// created by josevoz (T23) on 08/11/2020
//

#include "KickKeyframeOptimization_AgentTest.h"

void KickKeyframeOptimization_AgentTest::setup() {
    this->role = new decision_making::role::AttackingRole();
    wiz.setPlayMode(representations::RawPlayMode::PLAY_ON);
    double angle = -this->deltaAngle;
    this->ballOrigin = Vector3<double>(-8.0*cos(angle),8.0*sin(angle),0.3) + modeling.getWorldModel().getTheirGoalPosition();
    wiz.setBallPositionAndVelocity(this->ballOrigin, Vector3<double>(0,0,0));
    this->oldBallPos = wiz.getBallTranslation();
    wiz.setAgentPositionAndDirection(10, representations::PlaySide::LEFT, Vector3<double>(5,0,0.3), 0);
}

void KickKeyframeOptimization_AgentTest::runStep() {
    if(not decisionMaking.decideFall(modeling)) {
        role->execute(modeling);
        decisionMaking.lookRequest = role->getLookRequest();
        decisionMaking.movementRequest = role->getMovementRequest();
        decisionMaking.passRequest = role->getPassRequest();
        decisionMaking.sayMessage = role->getSayMessage();
    } else if(not this->ballMoved) {
        this->isFallen = true;
    }

    if(not this->startedKick) {
        this->startedKick = this->role->getBehaviorFactory().getKickBehavior().getState()
        == decision_making::behavior::KickBehavior::KICK_STATES::KICKING;
    } else {
        if(wiz.getBallTranslation().z > this->maxHeight) {
            this->maxHeight = wiz.getBallTranslation().z;
        }
        bool isNotPlayOn = modeling.getWorldModel().getPlayMode() != representations::PlayMode::PLAY_ON;
        bool exceededTime = this->time > this->TIME_LIMIT;
        bool finishedKicking = this->role->getBehaviorFactory().getKickBehavior().getState()
        != decision_making::behavior::KickBehavior::KICK_STATES::KICKING;
        bool ballStopped = (wiz.getBallTranslation() - this->oldBallPos).abs() < this->EPSILON;

        if(not ballStopped) {
            this->ballMoved = true;
        }

        if(isNotPlayOn or exceededTime or (ballStopped and finishedKicking)) {
            this->cumulativeReward += calculateReward();
            if(this->iterator < 5) {
                this->reset();
            } else {
                std::ofstream result = std::ofstream("interface.txt");
                result << this->cumulativeReward;
                result.flush();
                result.close();
                this->reset();
                this->isDone = true;
            }
        }
        this->time += 0.02;
        oldBallPos = wiz.getBallTranslation();
    }
}

double KickKeyframeOptimization_AgentTest::calculateReward() {
    double reward = 0.5*this->maxHeight;
    double areaXBoundary = modeling::FieldDescription::PENALTY_AREA_LENGTH;
    double areaYBoundary = modeling::FieldDescription::GOAL_AREA_WIDTH*1.5;
    Vector3<double> goalToBall = modeling.getWorldModel().getTheirGoalPosition() - wiz.getBallTranslation();

    if(goalToBall.x > 0 and goalToBall.x < areaXBoundary and std::abs(goalToBall.y) < areaYBoundary) {
        reward += 50;
    } else if(modeling.getWorldModel().getPlayMode() != representations::PlayMode::OPPONENT_GOAL_KICK and 
    modeling.getWorldModel().getPlayMode() != representations::PlayMode::PLAY_ON) {
        reward += 25;
    } else {
        reward += 8.0 - goalToBall.abs();
    }

    if(this->isFallen) {
        reward -= 1;
    }
    
    Vector3<double> goalPosition = modeling.getWorldModel().getTheirGoalPosition();

    double A=(goalPosition.y-this->ballOrigin.y)/(goalPosition.x-this->ballOrigin.x);
    double C=this->ballOrigin.y-A*this->ballOrigin.x;

    reward -= std::abs((A*wiz.getBallTranslation().x-1*wiz.getBallTranslation().y+C)/sqrt(A*A+1))*0.5;
    return reward;
}

void KickKeyframeOptimization_AgentTest::reset() {
    this->ballMoved = false;
    this->isFallen = false;
    this->iterator++;
    this->startedKick = false;
    wiz.setPlayMode(representations::RawPlayMode::PLAY_ON);
    wiz.setAgentPositionAndDirection(this->agentNumber, representations::PlaySide::LEFT, Vector3<double>(5,0,0.3), 0);
    double angle = (this->iterator%3 - 1)*this->deltaAngle;
    this->ballOrigin = Vector3<double>(-8.0*cos(angle),8.0*sin(angle),0.3) + modeling.getWorldModel().getTheirGoalPosition();
    wiz.setBallPositionAndVelocity(this->ballOrigin, Vector3<double>(0,0,0));
    wiz.setGameTime(0);
    this->time = 0;
    this->maxHeight = 0;
}

int main() {
    KickKeyframeOptimization_AgentTest agent;
    agent.testLoop();
}

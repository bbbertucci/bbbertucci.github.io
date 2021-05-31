import json
import numpy as np

class keyframe_page_handler:

    def __init__(self, file_name):
        self.file = open('configuration/core/control/keyframe/kick/front_kick/' + file_name + '.json', 'r+')
        self.file.seek(0)
        self.page = json.loads(self.file.read())
        self.name = file_name
        self.optimizable_params = ["leftShoulderPitch","leftShoulderYaw","leftArmRoll","leftHipYawpitch","leftHipRoll",
        "leftHipPitch","leftKneePitch","leftFootPitch","leftFootRoll","rightShoulderPitch","rightShoulderYaw","rightArmRoll",
        "rightHipYawpitch","rightHipRoll","rightHipPitch","rightKneePitch","rightFootPitch","rightFootRoll","delta"]
        self.speedrate = self.page['speedRate']
        self.interpolator = self.page['interpolator']
        self.steps = self.page['steps']
        self.numsteps = len(self.steps)
    
    def read_params(self):
        params = []
        for step in self.steps:
            for param in step:
                if param in self.optimizable_params:
                    params.append(step[param])
        return params

    def write_params(self, new_params):
        new_steps = []
        for i in range(self.numsteps):
            step = {'neckYaw' : 0.0, 'neckPitch' : 0.0}
            for j in range(3):
                index = i*len(self.optimizable_params) + j
                step[self.optimizable_params[j]] = new_params[index]
            step['leftArmYaw'] = 0.0
            for j in range(3,12):
                index = i*len(self.optimizable_params) + j
                step[self.optimizable_params[j]] = new_params[index]
            step['rightArmYaw'] = 0.0
            for j in range(12,19):
                index = i*len(self.optimizable_params) + j
                step[self.optimizable_params[j]] = new_params[index]
            new_steps.append(step)
        new_keyframe = {'name' : self.name,
        'speedRate' : self.speedrate,
        'interpolator' : self.interpolator,
        'steps' : new_steps}
        self.file.seek(0)
        self.file.truncate(0)
        self.file.write(json.dumps(new_keyframe))
        self.file.flush()
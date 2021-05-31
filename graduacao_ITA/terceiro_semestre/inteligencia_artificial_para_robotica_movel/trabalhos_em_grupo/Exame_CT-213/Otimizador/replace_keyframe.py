from keyframe_page_handler import keyframe_page_handler

def write_keyframe(x):
    index = 0
    prep.write_params(x[index:index+prep.numsteps*19])
    index += prep.numsteps*19
    back.write_params(x[index:index+back.numsteps*19])
    index += back.numsteps*19
    front.write_params(x[index:index+front.numsteps*19])

prep = keyframe_page_handler('prepFrontKickFast')
back = keyframe_page_handler('moveLegBack')
front = keyframe_page_handler('moveLegFront')

new_keyframe_file = open('best_sample.txt', 'r')
new_keyframe = new_keyframe_file.read().split()
new_keyframe[-1] = new_keyframe[-1][:-1]
new_keyframe = [float(param) for param in new_keyframe[1:]]
write_keyframe(new_keyframe)
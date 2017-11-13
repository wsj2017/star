#!/usr/bin/env python

import cv2
import numpy as np
import atag
import os
import sys
import time

if __name__ == '__main__':
    if len(sys.argv) <2:
        filename = 'a1.png'
    else:
        filename = sys.argv[1]
    frame = cv2.imread(filename)
    bg_frame = cv2.cvtColor(frame, cv2.COLOR_RGB2GRAY)
    bg_array = np.asarray(bg_frame)
    print "bg_array shape = {}".format(bg_array.shape)
    print type(bg_array)
    atag.init()
    t0 = time.time()
    ok, the_id, orien, cx, cy = atag.detect(bg_array)
    t1 = time.time()
    print "total time {} ms".format((t1-t0)*1000) 
    print "ok={}, id={}, orien={}, cx={}, cy={}".format(ok, the_id, orien, cx, cy)
    atag.cleanup()
    #cv2.imshow("Image", bg_frame)
    #cv2.waitKey(0)
    #cv2.destroyAllWindows()

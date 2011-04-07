#!/usr/bin/env python
import sys
#sys.path.insert(0,'..')
import drqueue.base.libdrqueue as drqueue
import os

print "Master: %s\n"%(os.environ["DRQUEUE_MASTER"])

computer_list = drqueue.request_computer_list (drqueue.CLIENT)
print "Computers connected to the master:"
for computer in computer_list:
    print "ID: %3i Name: %s"%(computer.hwinfo.id,computer.hwinfo.name.ljust(20))
    print "\nJobs registered in the master:"
    job_list = drqueue.request_job_list (drqueue.CLIENT)
    for job in job_list:
        print "ID: %3i Name: %s"%(job.id,job.name)

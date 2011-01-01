1. convert from ts to avi
mencoder 1.ts -ovc raw -vf format=i420 -o 1.avi -nosound
2. add_error is to add mosaic error to video
usage:
 add_error video errsize errcount
3.main target is libdetect.so.
4.msgserver create a msg queue and recv msg.
  ipcs -q : display all message queues
  ipcrm -q [msgid] : delete message queue by msg id
to test libdetect.so
1.run msgserver
# ./msgserver 
msgid = 12335
2.run demo (queue_id is given by msgserver)
# ./demo 12335 [video filename]




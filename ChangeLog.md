

                              mnthread (mini Thread) ChangeLog
Version 1.0.62 Mai 2020
  * update examples - stripped
  * switch from c3pb.gitlab to my public github account

Version 1.0.60 November 2018:
  * rename:
      * sleep -> mn_sleep
      * usleep -> mn_usleep
      * nsleep -> mn_nsleep
  * start thread mutext unlock funktion - see examples
  * update examples
  * update version
Version 1.0.5 November 2018:
  * Add libmn.h
  * Add Config : mn-config.h
  * rename header
  * Add examples

Version 0.9.3 November 2018:
  * add autolock_t to mn-Thread
  * add mn-autolock.h
  * add spinlock

Version 0.8.31 November 2018:
  * add logical mutext handle to basic_thread
  * rename function "delay" to "sleep" in basic_thread
  * add function "nsleep" and "usleep" to basic_thread
  * add "hild_thread" and "parent_thread" to basic_thread
  * add function "get_root();" "get_child();" "add_child_thread(basic_thread* thread);"

Version 0.8.3 November 2018:
 * add nsleep, sleep, usleep, micros, milis


Version 0.8.2, November 2018:
 * Public release
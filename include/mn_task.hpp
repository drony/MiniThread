/*
*This file is part of the Mini Thread Library (https://github.com/RoseLeBlood/MiniThread ).
*Copyright (c) 2018-2020 Amber-Sophia Schroeck
*
*The Mini Thread Library is free software; you can redistribute it and/or modify  
*it under the terms of the GNU Lesser General Public License as published by  
*the Free Software Foundation, version 3, or (at your option) any later version.

*The Mini Thread Library is distributed in the hope that it will be useful, but 
*WITHOUT ANY WARRANTY; without even the implied warranty of 
*MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
*General Public License for more details.
*
*You should have received a copy of the GNU Lesser General Public
*License along with the Mini Thread  Library; if not, see
*<https://www.gnu.org/licenses/>.  
*/
#ifndef MINLIB_ESP32_THREAD_
#define MINLIB_ESP32_THREAD_

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

#include "mn_autolock.hpp"
#include "mn_error.hpp"
#include "mn_sleep.hpp"
#include "mn_micros.hpp"
 

/**
 *  Wrapper class around FreeRTOS's implementation of a task.
 *
 *  This is an abstract base class.
 *  To use this, you need to subclass it. All of your task should
 *  be derived from the basic_task class. Then implement the virtual on_task
 *  function.
 */
class  basic_task {
public:
  enum priority {
    /**
     * no Real Time operation - idle task
     */ 
    PriorityIdle = MN_THREAD_CONFIG_CORE_PRIORITY_IDLE,
    /**
     * No critical operation
     */ 
    PriorityLow = MN_THREAD_CONFIG_CORE_PRIORITY_LOW,
    /**
     * Normal user programm
     */ 
    PriorityNormal = MN_THREAD_CONFIG_CORE_PRIORITY_NORM,
    /**
     * Half critical, this task have deadlines - not a lot of processings
     */ 
    PriorityHalfCritical = MN_THREAD_CONFIG_CORE_PRIORITY_HALFCRT,
    /**
     * Urgent critical, this task have short deadlines and a lot of processings
     */ 
    PriorityUrgent = MN_THREAD_CONFIG_CORE_PRIORITY_URGENT,
    /**
     * Critical, the highest priority 
     */ 
    PriorityCritical = MN_THREAD_CONFIG_CORE_PRIORITY_CRITICAL
  };
public:
  basic_task() { }

  /**
   *  Constructor for this task.
   *
   *  @param strName Name of the Task. Only useful for debugging.
   *  @param uiPriority FreeRTOS priority of this Task.
   *  @param usStackDepth Number of "words" allocated for the Task stack. default configMINIMAL_STACK_SIZE
   */
  basic_task(char const* strName, basic_task::priority uiPriority,
       unsigned short  usStackDepth = configMINIMAL_STACK_SIZE);
  
  
  /**
   *  Our destructor. Delete the task 
   */
	virtual ~basic_task();

  /**
   *  Create and starts the Task.
   *
   *  This is the API call that actually starts the Task running. 
   *  It creates a backing FreeRTOS task. By separating object creation 
   *  from starting the Task, it solves the pure virtual fuction call 
   *  failure case. Call after creating the Task the function on_create
   * 
   * @param uiCore If the value is MN_THREAD_CONFIG_CORE_IFNO, the created task is not
   * pinned to any CPU, and the scheduler can run it on any core available.
   * Other values indicate the index number of the CPU which the task should
   * be pinned to. Specifying values larger than (portNUM_PROCESSORS - 1) will
   * cause the function to fail.
   * 
   * 
   * @return ERR_TASK_OK The task are creating, 'ERR_TASK_CANTINITMUTEX' on error creating the using 
   * LockObjets, the task is not created, 'ERR_TASK_ALREADYRUNNING' the Task is allready running and
   * 'ERR_TASK_CANTSTARTTHREAD' can't create the task
   */
  virtual int                   create(int uiCore = MN_THREAD_CONFIG_DEFAULT_CORE);

  /**
   * Destroy and delete the task and call the function 'on_kill'
   * 
   * @return ERR_TASK_OK The tasx are destroyed and 'ERR_TASK_NOTRUNNING' the task is not running
   */ 
  int                   kill();

  /**
   * Is the Task  running?
   * 
   * @return true If the task  running, false If not
   */ 
  bool                  is_running();

  /**
   * Get the debug name of this task  
   * 
   * @return The name of this task  
   */ 
  const char*           get_name();
  /**
   * Get the priority of this task  
   * 
   * @return The priority
   */ 
  basic_task::priority get_priority();
  /**
   * Get the stack depth of this task  
   * 
   * @return The stack depth
   */ 
  unsigned short        get_stackdepth();
  /**
   * Accessor to get the task's backing task handle.
   * There is no setter, on purpose.
   * 
   * @return FreeRTOS task handle.
   */ 
  xTaskHandle           get_handle();
  /**
   * Get the return value of this task  - after run 
   * 
   * @return The return value 
   */
  void*                 get_return_value();
  /**
   * Get the time since start of this task  
   * 
   * @return The time since start of this task  
   */ 
  uint32_t              get_time_since_start();
  /**
   * Get the FreeRTOS task Numberid of this task  
   * 
   * @return The FreeRTOS task   Number
   */ 
  uint32_t              get_id();
  /**
   * Get the core number of this task  run
   * 
   * @return The core number
   */ 
  uint32_t              get_on_core();

  /**
   *  Set the priority of this task.
   *
   *  @param uiPriority The task's new priority.
   */
  void                  set_priority(basic_task::priority uiPriority);

  /**
   *  Suspend this task.
   *
   *  @note While a task can suspend() itself, it cannot resume() 
   *  itself, becauseit's suspended.
   */
  void                  suspend();
  /**
   *  Resume a specific task.
   */
  void                  resume();

  /**
   * This virtual function call on creating, use for user code
   * It is optional whether you implement this or not.
   */ 
  virtual void          on_create() {  }
  /**
   * This virtual function call on kill, use for user code
   * It is optional whether you implement this or not.
   */ 
  virtual void          on_kill()   {  }

  /**
   * Implementation of your actual task code.
   * You must override this function.
   * 
   * @return Your return your task function, get with get_return_value()
   */
  virtual void*         on_task() = 0;

  /**
   *  Called on exit from your on_task() routine. 
   *  
   *  It is optional whether you implement this or not.
   *
   *  If you allow your task to exit its on_task method, 
   */ 
  virtual void          on_cleanup() { }

  /**
   * Get the root task of this task list
   * 
   * @return The root task
   */ 
  basic_task*         get_root();
  /**
   * Get the child task of this task
   * 
   * @return The child task
   */ 
  basic_task*         get_child();

  /**
   * Add a child task to this task. 
   * 
   * @return True The child tasx are add and false when not
   * 
   * @note For example this task handle the WiFi connection and the child the TCP Connection
   * on signal or broadcast this task, will signal and broadcast the child too.
   */ 
  bool                  add_child_task(basic_task* task);

public:
  /**
   * Suspend the given task.
   *
   * @param t The given task to suspend
   * 
   * @note While a task can suspend() itself, it cannot resume() 
   * itself, becauseit's suspended.
   */
  static void suspend(basic_task *t)  { t->suspend(); }

  /**
   * Resume the given task.
   *
   * @param t The given task to resume
   */
  static void resume(basic_task *t)   {   t->resume(); }

  /**
   *  Yield the scheduler.
   */
  static void yield()                   { taskYIELD(); }
  /**
   *  sleep this task for n seconds
   *
   *  @param secs How long seconds to sleep the task.
   */
  static void sleep(unsigned int secs)     { ::mn_sleep(secs); }
  /**
   *  sleep this task for n micro seconds
   *
   *  @param secs How long micro seconds to sleep the task.
   */
  static void usleep(unsigned int usec)     { ::mn_usleep(usec); }
  /**
   * pause execution for a specified time
   * @note see Linux nanosleep function
  */
  static void nsleep(const struct timespec *req, struct timespec *rem)     {
    ::mn_nsleep(req, rem);
  }

  static void lock(basic_task * t)    { t->m_runningMutex->lock(); }
  static void unlock(basic_task * t)    { t->m_runningMutex->unlock(); }

protected:
  /**
   *  Adapter function that allows you to write a class
   *  specific on_task() function that interfaces with FreeRTOS.
   */
  static void runtaskstub(void* parm);
  /**
   * Internal function 
   */ 
	void task_started();

protected:
  /**
   *  Reference to the underlying task handle for this task.
   *  Can be obtained from GetHandle().
   */
  xTaskHandle handle;
  /**
   *  The name of this task.
   */
  const char* m_strName;
  /**
   *  A saved / cached copy of what the task's priority is.
   */
  basic_task::priority m_uiPriority;
  /**
   *  Stack depth of this task, in words.
   */
  unsigned short m_usStackDepth;
  /**
   * The return value from user task routine
   */ 
  void* m_retval;

  /**
   *  Flag whether or not the LockObject was created.
   */
  bool m_bMutexInit;
  /**
   *  Flag whether or not the task was started.
   */
  bool m_bRunning;
  /**
   * The FreeRTOS task Number
   */ 
  uint32_t m_iID;
  /**
   * A saved / cached copy of which core this task is running on
   */ 
  uint32_t m_iCore;
  /**
   * Lock Objekt for task safty
   */ 
  LockType_t *m_runningMutex;
  /**
   * Lock Objekt for task safty
   */ 
  LockType_t *m_contextMutext;
  /**
   * Lock Objekt for task safty
   */ 
  LockType_t *m_continuemutex, *m_continuemutex2;

  /**
   * The child task pointer
   */ 
  basic_task *m_pChild;
  /**
   * The parent task pointer of this task
   */
  basic_task *m_pParent;
};

using task_t = basic_task;




#endif
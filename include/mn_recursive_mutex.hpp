#ifndef MINLIB_ESP32_REC_MUTEX_
#define MINLIB_ESP32_REC_MUTEX_

#include "mn_mutex.hpp"

class recursive_mutex : public basic_mutex {
public:
  recursive_mutex();
  ~recursive_mutex();

  virtual int create();
	virtual int lock();
	virtual int unlock();

  virtual bool try_lock();
};

using remutex_t = recursive_mutex;

#endif

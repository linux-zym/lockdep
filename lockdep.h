#ifndef LOCKDEP_H
#define LOCKDEP_H

#include <set>
#include <string>
#include <utility>


#ifdef __cplusplus
extern "C" {
#endif

typedef pthread_mutex_t mutex_t;
#define MUTEX_INIT PTHREAD_MUTEX_INITIALIZER

static inline void mutex_lock(mutex_t *m) { pthread_mutex_lock(m); }
static inline void mutex_unlock(mutex_t *m) { pthread_mutex_unlock(m); }

typedef struct lock_t {
    mutex_t mutex;
    const char *name;
} lock_t;

#define STRINGIFY(s) #s
#define TOSTRING(s) STRINGIFY(s)
#define LOCK_INIT (lock_t){ MUTEX_INIT, __FILE__ ":" TOSTRING(__LINE__) }

void lock(lock_t *lk);
void unlock(lock_t *lk);

#ifdef __cplusplus
}
#endif

#endif // LOCKDEP_H
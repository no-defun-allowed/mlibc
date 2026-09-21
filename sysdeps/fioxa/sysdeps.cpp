#include <cstddef>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <mlibc/all-sysdeps.hpp>

#define STUB(NAME) { fioxa_log("Hit a stub " NAME); fioxa_panic(); }

extern "C" {
  extern void fioxa_log(const char*);
  [[noreturn]] extern void fioxa_panic();

  extern int fioxa_close(int);
  extern int fioxa_clock(time_t*, long*);
  [[noreturn]] extern void fioxa_exit(int);
  extern int fioxa_isatty(int);
  extern int fioxa_map(size_t, void**);
  extern int fioxa_open(const char*, int, mode_t, int*);
  extern int fioxa_read(int, void*, size_t, ssize_t*);
  extern int fioxa_seek(int, off_t, int, off_t*);
  extern int fioxa_sleep(long, long);
  extern void fioxa_set_fs_reg(void*);
  extern int fioxa_unmap(void*, size_t);
  extern int fioxa_write(int, const void*, size_t);
}

namespace mlibc {
  void Sysdeps<LibcLog>::operator()(const char *message) { fioxa_log(message); }
  [[noreturn]] void Sysdeps<LibcPanic>::operator()() { fioxa_panic(); }

  int Sysdeps<AnonAllocate>::operator()(size_t size, void **pointer) { return fioxa_map(size, pointer); }
  int Sysdeps<AnonFree>::operator()(void *pointer, size_t size) { return fioxa_unmap(pointer, size); }
  int Sysdeps<ClockGet>::operator()(int clock, time_t *seconds, long *nanoseconds) {
    return fioxa_clock(seconds, nanoseconds);
  }
  int Sysdeps<Close>::operator()(int fd) { return fioxa_close(fd); }
  void Sysdeps<Exit>::operator()(int status) { fioxa_exit(status); }
  int Sysdeps<FutexWait>::operator()(int *pointer, int expected, const struct timespec *timeout) { STUB("wait"); }
  int Sysdeps<FutexWake>::operator()(int *pointer, bool all) { STUB("wake"); }
  int Sysdeps<Isatty>::operator()(int fd) { return fioxa_isatty(fd); }
  int Sysdeps<Open>::operator()(const char* path, int flags, mode_t mode, int *fd) {
    return fioxa_open(path, flags, mode, fd);
  }
  int Sysdeps<Read>::operator()(int fd, void *buf, size_t count, ssize_t *bytes_read) {
    return fioxa_read(fd, buf, count, bytes_read);
  }
  int Sysdeps<Seek>::operator()(int fd, off_t offset, int whence, off_t *new_offset) {
    return fioxa_seek(fd, offset, whence, new_offset);
  }
  int Sysdeps<Sleep>::operator()(time_t *secs, long *nanos) {
    return fioxa_sleep(*secs, *nanos);
  }
  int Sysdeps<TcbSet>::operator()(void *pointer) { fioxa_set_fs_reg(pointer); return 0; }
  int Sysdeps<VmMap>::operator()(void *hint, size_t size, int prot, int flags, int fd, off_t offset, void **window) {
    if (fd != -1) return -ENOSYS;
    return fioxa_map(size, window);
  }
  int Sysdeps<VmUnmap>::operator()(void *pointer, size_t size) { return fioxa_unmap(pointer, size); }
  int Sysdeps<Write>::operator()(int fd, const void *buf, size_t count, ssize_t *bytes_written) {
    int ret = fioxa_write(fd, buf, count);
    *bytes_written = count;
    return ret;
  }

  static_assert(std::same_as<Sysdeps<Sigaction>, NoImpl>);
  static_assert(!IsImplemented<Sigaction>);
}

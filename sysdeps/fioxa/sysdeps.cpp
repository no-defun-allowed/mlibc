#include <cstddef>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>

#define STUB(NAME) { sys_libc_log("Hit a stub " NAME); sys_libc_panic(); }

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
  void sys_libc_log(const char *message) { fioxa_log(message); }
  [[noreturn]] void sys_libc_panic() { fioxa_panic(); }
  
  int sys_anon_free(void *pointer, size_t size) { return fioxa_unmap(pointer, size); }
  int sys_clock_get(int clock, time_t *seconds, long *nanoseconds) {
    return fioxa_clock(seconds, nanoseconds);
  }
  int sys_close(int fd) { return fioxa_close(fd); }
  int sys_exit(int status) { fioxa_exit(status); }
  int sys_futex_wait(int *pointer, int expected, const struct timespec *timeout) { STUB("wait"); }
  int sys_futex_wake(int *pointer) { STUB("wake"); }
  int sys_isatty(int fd) { return fioxa_isatty(fd); }
  int sys_open(const char* path, int flags, mode_t mode, int *fd) {
    return fioxa_open(path, flags, mode, fd);
  }
  int sys_read(int fd, void *buf, size_t count, ssize_t *bytes_read) {
    return fioxa_read(fd, buf, count, bytes_read);
  }
  int sys_seek(int fd, off_t offset, int whence, off_t *new_offset) {
    return fioxa_seek(fd, offset, whence, new_offset);
  }
  int sys_sleep(time_t *secs, long *nanos) {
    return fioxa_sleep(*secs, *nanos);
  }
  int sys_tcb_set(void *pointer) { fioxa_set_fs_reg(pointer); return 0; }
  int sys_vm_map(void *hint, size_t size, int prot, int flags, int fd, off_t offset, void **window) {
    if (fd != -1) return -ENOSYS;
    return fioxa_map(size, window);
  }
  int sys_vm_unmap(void *pointer, size_t size) { return fioxa_unmap(pointer, size); }
  int sys_write(int fd, const void *buf, size_t count, ssize_t *bytes_written) {
    int ret = fioxa_write(fd, buf, count);
    *bytes_written = count;
    return ret;
  }

  int sys_anon_allocate(size_t size, void **pointer) {
    return sys_vm_map(NULL, size, PROT_READ | PROT_WRITE, MAP_ANON, -1, 0, pointer);
  }
}

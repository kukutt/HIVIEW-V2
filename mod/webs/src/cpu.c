#include "webs.h"

#include "fw/comm/inc/sstat.h"

static pthread_t tid = 0;
static float _cpu, _temp, _mem, _ddr;
void* stat_task(void *parm)
{  
  pthread_detach(pthread_self());

  while(1)
  {
#if defined(GSF_CPU_3516e) || defined(GSF_CPU_x86)
    _cpu = 0;
    _mem = 0;
    _temp = 0;
    _ddr = 0;
#else
    _cpu = sstat_cpu_occupy(1);
    _mem = sstat_mem_occupy();
    _temp = sstat_cpu_temp(1);
    _ddr = sstat_ddr_occupy(3);
#endif
    sleep(1);
  }
  return 0;
}

int sys_stat(float *cpu, float *temp, float *mem, float *ddr)
{
  if(!tid)
    pthread_create(&tid, NULL, stat_task, (void*)NULL);
    
  *cpu = _cpu;
  *temp = _temp;
  *mem = _mem;
  *ddr = _ddr;
  return 0; 
}

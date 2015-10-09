#include "factory.h"
#include "../log.h"

using namespace module::logger;

namespace module
{
namespace factory
{

SharedObjectsFactory SharedObjectsFactory::m_instance;

bool SharedObjectsFactory::Initialize()
{
	/*
	 * initialize using POSIX shm
	 */
	int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
	if(shm_fd < 0)
	{
		LOG_WARN_WITH_NO("shm_open");
		return false;
	}

	void* addr = mmap(NULL, sizeof(struct SharedMemory), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if(addr == MAP_FAILED)
	{
		LOG_CRITICAL_WITH_NO("mmap");
		return false;
	}

	this->m_addr = (struct SharedMemory*)addr;

    /*
     * init semaphores
     * if succeeded, return true, as it's the final process here
     * else return false
     */
    return initSems();
}

void SharedObjectsFactory::Destroy()
{
    destroySems();
	if(m_addr != NULL)
	{
		munmap(m_addr, sizeof(struct SharedMemory));
		m_addr = NULL;
    }
}

bool SharedObjectsFactory::initSems()
{
    //init semaphores for metadata
    for( int i = 0; i < SEM_META_NUM; ++i){
        if(sem_init(m_addr->shm_metaData.sems[i], 1, 0) == -1){
            perror("sem_init for metadata");
            LOG_CRITICAL_WITH_NO("sem_init for metadata");
            return false;
        }
    }
    //init semaphores for others to be added...

    return true;
}

void SharedObjectsFactory::destroySems()
{
    //destory semaphores for metadata
    for( int i = 0; i < SEM_META_NUM; ++i){
        if((m_addr->shm_metaData.sems[i] != NULL)
                && (sem_destroy(m_addr->shm_metaData.sems[i]) == -1)){
            perror("sem_destroy for metadata");
            LOG_WARN_WITH_NO("sem_destroy for metadata");
        }
    }
    //destory semaphores for others to be added...
}

}
}

/*************************************************************************
	> File Name: CacheThread.cpp
	> Author: Dubyoo
	> Mail:wangwei1543@gmail.com 
	> Created Time: Sat 10 May 2014 01:07:53 AM CST
 ************************************************************************/

#include "CacheThread.h"
#include "ThreadPool.h"

void CacheThread::regist_thread_pool(ThreadPool * p_thread_pool)
{
	m_p_thread_pool = p_thread_pool; //注册到线程池
}

void CacheThread::run()
{
	read_disk_cache();	//程序启动时，读磁盘cache
	while(true)
	{
		if(!m_p_thread_pool -> m_is_started) //线程池关闭时，退出
		{
			CacheThread::join();
		}
#ifndef NDEBUG
	std::cout << "------ sync start!~ ------" << std::endl;
#endif
		m_p_thread_pool -> get_cache(m_cachethread_cache); //与线程池cache同步
		write_disk_cache(); //写磁盘cache（覆盖）
		sleep(10);
	}
}

void CacheThread::read_disk_cache()	//读磁盘cache
{
	//读配置文件，获得磁盘cache文件的路径
	std::ifstream ifs_conf("./conf/config.txt");
	if(!ifs_conf)
	{
#ifndef NDEBUG
		std::cout << "!!!!!! \"./conf/config.txt\" ifs OPEN FAILED !!!!!!" << std::endl;
#endif
	}
	std::string line, conf, file_name;
	while(getline(ifs_conf, line))
	{
		std::istringstream iss(line);
		iss >> conf;
		if(conf == "cache:")
		{
			iss >> file_name;
			break;
		}
	}
	ifs_conf.close();
	//获取到cache文件路径，开始读cache
	std::ifstream ifs(file_name.c_str());
	if(!ifs)
	{
#ifndef NDEBUG
		std::cout << "!!!!!! \"" << file_name << "\" ifs OPEN FAILED !!!!!!" << std::endl;
#endif
	}
	std::string word_in, word_out;
	while(getline(ifs, line))
	{
		std::istringstream iss(line);
		iss >> word_in >> word_out;
		m_cachethread_cache[word_in] = word_out;
	}
	ifs.close();
}

void CacheThread::write_disk_cache() //写磁盘cache
{
	std::ifstream ifs_conf("./conf/config.txt");
	if(!ifs_conf)
	{
#ifndef NDEBUG
		std::cout << "!!!!!! \"./conf/config.txt\" ifs OPEN FAILED !!!!!!" << std::endl;
#endif
	}
	std::string line, conf, file_name;
	while(getline(ifs_conf, line))
	{
		std::istringstream iss(line);
		iss >> conf;
		if(conf == "cache:")
		{
			iss >> file_name;
			break;
		}
	}
	ifs_conf.close();
	std::ofstream ofs(file_name.c_str());
	if(!ofs)
	{
#ifndef NDEBUG
		std::cout << "!!!!!! \"" << file_name << "\" ifs OPEN FAILED !!!!!!" << std::endl;
#endif
	}
	for(std::unordered_map<std::string, std::string>::iterator iter = m_cachethread_cache.begin(); iter != m_cachethread_cache.end(); ++iter)
	{
		ofs << iter->first << " " << iter->second << std::endl;
	}
	ofs.close();
}


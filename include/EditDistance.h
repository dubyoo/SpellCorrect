/*************************************************************************
	> File Name: EditDistance.h
	> Author: Dubyoo
	> Mail:wangwei1543@gmail.com 
	> Created Time: Mon 05 May 2014 08:25:18 PM CST
 ************************************************************************/

#ifndef __EDITDISTANCE_H__
#define __EDITDISTANCE_H__

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <queue>
#include <string.h>
#include <stdint.h>
#include <string>
#include <sstream>
#include <utility>
#include <vector>
#include <unordered_map>

struct WordResult
{
	std::string m_word;
	int m_freq;
	int m_ed;
};

struct compare
{
	bool operator()(const WordResult &a, const WordResult &b)
	{
		if(a.m_ed > b.m_ed)
			return true;
		else if(a.m_ed == b.m_ed && a.m_freq < b.m_freq)
			return true;
		else 
			return false;
	}
};

class EditDistance
{
	public:
		EditDistance(const std::string &file_name);
		std::string compute_edit_distance(const std::string &word);
	private:
		std::priority_queue<WordResult, std::vector<WordResult>, compare > m_queue;
		std::unordered_map<uint32_t, std::map<std::string, int> > m_index;
};

#endif

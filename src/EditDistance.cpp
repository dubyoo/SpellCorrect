/*************************************************************************
	> File Name: EditDistance.cpp
	> Author: Dubyoo
	> Mail:wangwei1543@gmail.com 
	> Created Time: Mon 05 May 2014 08:33:57 PM CST
 ************************************************************************/

#include "EditDistance.h"

/* 
 * utf-8 编码中文字符格式占3个字节，编码格式如下
 * 第一字节 第二字节 第三字节
 * 1110xxxx 10xxxxxx 10xxxxxx
 *
 * 英文字符，数字等占一个字节，格式如下
 * 0xxxxxxx
 *
 * */


static int get_min(int a, int b, int c)
{
	return (a < b ? a : b) < c ? (a < b ? a : b) : c ;
}
static bool is_utf8_ch_first(unsigned char c) //判断是否utf-8中文的第一字节
{
	return ((c & 0xE0) == 0xE0);
}

static bool is_utf8_ch_last(unsigned char c) //判断是否utf-8中文的后2字节
{
	return ((c & 0x80) == 0x80);
}

static bool is_en(unsigned char c) //判断是否英文字节
{
	return ((c & 0x80) == 0);
}

static void parse_utf8_string(const std::string &s, std::vector<uint32_t> &vec) //将utf-8编码的字符串转化为32位整形数数组（每个字转化为一个数字）
{
	for(std::string::size_type ix = 0; ix != s.size(); ++ix)
	{
		if(is_en(s[ix]))	//当前字节为英文
		{
			vec.push_back((uint32_t)(s[ix]));
		}
		else if(is_utf8_ch_first(s[ix])) //当前字节为中文的第一字节
		{
			if(is_utf8_ch_last(s[ix + 1]) && is_utf8_ch_last(s[ix + 2]))
			{
				vec.push_back((uint32_t)(s[ix] << 16) + (uint32_t)(s[ix + 1] << 8) +(uint32_t)(s[ix + 2]));
				++ix;
				++ix;
			}
			else	//非utf-8中文或英文情况的处理
			{
#ifndef NDEBUG
				std::cout << "not utf-8 character" << std::endl;
#endif
				++ix;
				while((!is_utf8_ch_first(s[ix + 1])) && (!is_en(s[ix + 1])))
				{
					++ix;
				}
				//throw runtime_error("utf-8");
			}
		}
	}
}

static int EditDistance_utf8(const std::string &a, const std::string &b) //计算utf-8字符串的编辑距离
{

	std::vector<uint32_t> vec_a;
	std::vector<uint32_t> vec_b;
	parse_utf8_string(a, vec_a); //将a，b字符串转化为32位整型数数组
	parse_utf8_string(b, vec_b);
	int len_a = vec_a.size();
	int len_b = vec_b.size();
	int **memo = new int * [len_a + 1]; //动态构造二维数组
	for(int ix = 0; ix != len_a + 1; ++ix)
	{
		memo[ix] = new int [len_b + 1];
	}
	for(int ix = 0; ix != len_a + 1; ++ix) //初始化第一行
	{
		memo[ix][0] = ix;
	}
	for(int jx = 0; jx != len_b +1; ++jx) //初始化第一列
	{
		memo[0][jx] = jx;
	}
	for(int ix = 1; ix != len_a +1; ++ix)
	{
		for(int jx = 1; jx != len_b +1; ++jx)
		{
			if(vec_a[ix - 1] == vec_b[jx - 1]) //当前字相等时，取leftup值
			{
				memo[ix][jx] = memo[ix -1][jx -1];
			}
			else //不相等时，取 left, up, leftup 三者中最小值 +1
			{
				memo[ix][jx] = get_min(memo[ix - 1][jx] + 1,
									   memo[ix][jx - 1] + 1,
									   memo[ix - 1][jx - 1] +1);
			}
		}
	}
	int iret = memo[len_a][len_b]; //返回值
	for(int ix = 0; ix != len_a; ++ix)
	{
		delete [] memo[ix];
	}
	delete [] memo;
	return iret;
}

static void make_index(const std::map<std::string, int> &word_lib, std::unordered_map<uint32_t, std::map<std::string, int> > &index) //建索引
{
	for(std::map<std::string, int>::const_iterator iter_map = word_lib.begin(); iter_map != word_lib.end(); ++iter_map) //遍历词库map
	{
		std::vector<uint32_t> tmp_vec; 
		parse_utf8_string(iter_map->first, tmp_vec); //将map中每一个单词拆分成32位整型数数组
		for(std::vector<uint32_t>::iterator iter_vec = tmp_vec.begin(); iter_vec != tmp_vec.end(); ++iter_vec) //遍历该数组
		{
			index[*iter_vec].insert(make_pair(iter_map->first, iter_map->second)); 
				//对数组中的每一个数建立一个对应的索引map
			
		}
	}
}

EditDistance::EditDistance(const std::string &file_name)
{
	std::ifstream ifs(file_name.c_str());
	if(!ifs)
	{
#ifndef NDEBUG
		std::cout << "!!!!!! \"" << file_name << "\" OPEN FAILED !!!!!!" << std::endl;
#endif
	}
	std::string line;
	std::string word;
	std::map<std::string, int> word_lib_map;
	int freq;
	while(getline(ifs, line)) //从词库中读出单词和词频，存入map中
	{
		std::istringstream iss(line);
		iss >> word >> freq;
		word_lib_map[word] = freq;
	}
	ifs.close();
	make_index(word_lib_map, m_index); //根据上面的map建索引
}


std::string EditDistance::compute_edit_distance(const std::string &word) //输入一个单词，返回词库中与之匹配的编辑距离最小的（词频最高的）单词
{
	std::map<std::string, int> word_compare_map;
	std::vector<uint32_t> tmp_vec;
	parse_utf8_string(word, tmp_vec); //将输入的单词转换成一个32位整型数数组
	for(std::vector<uint32_t>::iterator iter_vec = tmp_vec.begin(); iter_vec != tmp_vec.end(); ++iter_vec)
	{
		for(std::map<std::string, int>::iterator iter_map = m_index[*iter_vec].begin(); iter_map != m_index[*iter_vec].end(); ++iter_map)
		{
			word_compare_map.insert(make_pair(iter_map->first, iter_map->second)); //在索引中找出具有公共字的对应map，复制这些map，构成一个新的map，此时只需要比较计算该map中的单词和词频
		}
	}
	
	for(std::map<std::string, int>::iterator iter = word_compare_map.begin(); iter != word_compare_map.end(); ++iter)
	{
		WordResult wd;
		wd.m_ed = EditDistance_utf8(iter->first, word); //计算utf-8编码字符串的编辑距离
		wd.m_freq = iter -> second;
		wd.m_word = iter -> first;
		m_queue.push(wd);	//将结果放入优先级队列
		if(wd.m_ed == 0)
		{
			break;
		}
	}
	std::string ret;
		char frq[128];
		memset(frq, 0, 128);
		ret = m_queue.top().m_word;
	//if(m_queue.top().m_ed >=3 || m_queue.top().m_ed >= (int)word.size()) //抛弃编辑距离大于3的词，并返回"none!"
	//{
	//	ret = "none!";
	//}
	while(!m_queue.empty())		//完成后清空优先级队列，否则重复调用该优先级队列时可能会导致错误
	{
		m_queue.pop();
	}
	return ret;
}


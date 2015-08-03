#ifndef __CIRCLE_BUFFER_H__
#define __CIRCLE_BUFFER_H__
#include <cstdint>
#include <stdexcept>

template <typename T>
class circle_buffer
{
public:
	explicit circle_buffer(uint64_t sz = 50000):size_(sz), wr_index(0), rd_index(0)
	{
		if( sz < 2 )
		{
			throw std::runtime_error("too few elements, size must grater than 1");
		}
		array = new T[sz + 1];
	}
	
	~circle_buffer()
	{
		delete [] array;
		array = NULL;
	}
public:
	bool  push_back(const T& val)
	{
		if (full())
			return false;
		array[wr_index] = val;
		wr_index = inc(wr_index);
		return true;
	}

	bool pop_front(T* val = 0)
	{
		if(empty() || val == NULL)
			return false;
		*val = array[rd_index];
		rd_index = inc(rd_index);
		return true;
	}

	T* get_write_index_memory()
	{
		if(full())
			return NULL;
		return &array[wr_index];
	}


	bool put_write_index_memory()
	{
		if(full())
			return false;
		wr_index = inc(wr_index);
	}

	T* get_read_index_memory()
	{
		if(empty())
			return NULL;
		return &array[rd_index];
	}

	bool put_read_index_memory()
	{
		if(empty())
			return false;
		rd_index = inc(rd_index);
		return true;
	}

	size_t size()
	{
		int n = wr_index - rd_index;
		return n < 0? n + size_ : n;
	}

	bool empty() const
	{
		return wr_index == rd_index;
	}

	bool full () const 
	{
		return  inc(wr_index) == rd_index;
	}
private:
	uint64_t  inc(uint64_t n) const
	{
		return ++n % (size_ + 1);
	}


private:
	T* array;
	
	const uint64_t size_;
	uint64_t wr_index;
	uint64_t rd_index;

};

#endif

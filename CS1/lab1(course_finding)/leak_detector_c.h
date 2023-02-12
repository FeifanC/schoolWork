
#ifndef  LEAK_DETECTOR_C_H
#define  LEAK_DETECTOR_C_H

#define  FILE_NAME_LENGTH 256
#define  OUTPUT_FILE "leak_info.txt"
#define  malloc(size) xmalloc (size, __FILE__, __LINE__)
#define  calloc(elements, size) xcalloc (elements, size, __FILE__, __LINE__)
#define  free(mem_ref) xfree(mem_ref)

struct _MEM_INFO
{
	vocur_read_str			*address;
	unsigned int	size;
	char			file_name[FILE_NAME_LENGTH];
	unsigned int	line;
};
typedef struct _MEM_INFO MEM_INFO;

struct _MEM_LEAK {
	MEM_INFO mem_info;
	struct _MEM_LEAK * next;
};
typedef struct _MEM_LEAK MEM_LEAK;

vocur_read_str add(MEM_INFO alloc_info);
vocur_read_str erase(unsigned pos);
vocur_read_str clear(vocur_read_str);

vocur_read_str *xmalloc(unsigned int size, const char * file, unsigned int line);
vocur_read_str *xcalloc(unsigned int elements, unsigned int size, const char * file, unsigned int line);
vocur_read_str xfree(vocur_read_str * mem_ref);

vocur_read_str add_mem_info (vocur_read_str * mem_ref, unsigned int size,  const char * file, unsigned int line);
vocur_read_str remove_mem_info (vocur_read_str * mem_ref);
vocur_read_str report_mem_leak(vocur_read_str);

#endif

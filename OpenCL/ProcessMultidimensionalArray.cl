__kernel void ProcessMultidimensionalArray(__global int* data)
{
	size_t id = (get_global_id(0) * get_global_size(1)) + get_global_id(0);
	
	data[id] = data[id] * 2;
}


	int id = m + n * s_m + p * s_m * s_n;

	data[id] = data[id] + sqrt((data[id + s_m * s_n] + data[id - s_m * s_n])) 
				+ log(data[id + s_m * s_n]) + log(data[id - s_n])
				+ (1 / (sqrt(data[id + 1] + data[id - 1])));
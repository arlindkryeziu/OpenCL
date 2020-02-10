__kernel void Process3DArray(__global double* data)
{
	int m = get_global_id(0);
	int n = get_global_id(1);
	int p = get_global_id(2);
	
	int s_m = get_global_size(0);
	int s_p = get_global_size(2);
	
	if (m > 0 && m < s_m - 1 && n > 0 && n < s_m - 1 && p == 1)
	{
		int id = s_p * s_m * m + s_p * n + p;
		
		int id_m0 = s_p * s_m * (m + 1) + s_p * n + p;
		int id_m1 = s_p * s_m * (m - 1) + s_p * n + p;
		int id_n0 = s_p * s_m * m + s_p * (n + 1) + p;
		int id_n1 = s_p * s_m * m + s_p * (n - 1) + p;
		int id_p0 = s_p * s_m * m + s_p * n + (p + 1);
		int id_p1 = s_p * s_m * m + s_p * n + (p - 1);
		
		data[id] = data[id] + sqrt(data[id_p0] + data[id_p1]) 
							+ log(data[id_n0]) + log(data[id_n1]) 
							+ (1 / (sqrt(data[id_m0] + data[id_m1])));
	}
}
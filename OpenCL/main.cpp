#define CL_USE_DEPRECATED_OPENCL_2_0_APIS

#include <CL/cl.hpp>
#include <fstream>
#include <iostream>
#include <math.h>
#include <ctime>
#include <array>

using namespace std;
using namespace cl;

Program CreateProgram(const string& file);

int main()
{
	const int times = 1; //24

	const int m = 100; //8192
	const int n = 100; //8192
	const int p = 3; //3
	const int count = n * m * p;

	double a[m][n][p];
	double b[m][n][p];

	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			a[i][j][0] = i / (j + 1);
			a[i][j][1] = 1.00;
			a[i][j][2] = j / (i + 1);

			b[i][j][0] = i / (j + 1);
			b[i][j][1] = 1.00;
			b[i][j][2] = j / (i + 1);
		}
	}

	time_t start_time = time(NULL);

	for (int t = 0; t < times; t++)
	{
		for (int i = 1; i < m - 1; i++)
		{
			for (int j = 1; j < n - 1; j++)
			{
				a[i][j][1] = a[i][j][1] + sqrt((a[i][j][0] + a[i][j][2])) + log(a[i][j + 1][1])
							+ log(a[i][j - 1][1]) + (1 / (sqrt(a[i + 1][j][1] + a[i - 1][j][1])));
			}
		}
	}

	time_t end_time = time(NULL);

	auto program = CreateProgram("Process3DArray.cl");
	//auto program = CreateProgram("ProcessMultidimensionalArray.cl");
	auto context = program.getInfo<CL_PROGRAM_CONTEXT>();
	auto devices = context.getInfo<CL_CONTEXT_DEVICES>();
	auto& device = devices.front();

	//int arr[3][2] = { {1,1}, {2,2}, {3,3} };
	//Buffer buff(context, CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * 6, arr);
	//Kernel kernel(program, "ProcessMultidimensionalArray");
	//kernel.setArg(0, buff);

	//CommandQueue queue(context, device);
	//queue.enqueueNDRangeKernel(kernel, NullRange, NDRange(2, 3));
	//queue.enqueueReadBuffer(buff, GL_TRUE, 0, sizeof(int) * 6, arr);

	Buffer buff(context, CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(double) * count, b);
	Kernel kernel(program, "Process3DArray");
	kernel.setArg(0, buff);

	for (int t = 0; t < times; t++)
	{
		CommandQueue queue(context, device);
		queue.enqueueNDRangeKernel(kernel, NullRange, NDRange(m, n, p));
		queue.enqueueReadBuffer(buff, GL_TRUE, 0, sizeof(double) * count, b);
	}
	
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (a[i][j][1] != b[i][j][1])
			{
				cin.get();
			}
		}
	}

	finish();
	cin.get();
}

Program CreateProgram(const string& file) {
	vector<Platform> platforms;
	Platform::get(&platforms);

	auto platform = platforms.front();
	vector<Device> devices;

	platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
	auto device = devices.front();

	ifstream operations_cl_file(file);
	string src(istreambuf_iterator<char>(operations_cl_file), (istreambuf_iterator<char>()));

	Program::Sources sources(1, make_pair(src.c_str(), src.length() + 1));

	Context context(device);
	Program program(context, sources);

	program.build("-cl-std=CL1.2");
	return program;
}
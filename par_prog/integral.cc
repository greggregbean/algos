#include <iostream>
#include <cmath>
#include <vector>
#include <thread>
#include <atomic>

std::atomic<double> result = 0.0;

double f (double x)
{
    return sin (1 / x);
}

void count_integral (double from, double to)
{
    unsigned long num_of_points = 1000000;
    double step = (to - from) / num_of_points;
    double sum = 0.0;

    for (int i = 0; i < num_of_points; ++i) 
    {
        sum += f(from + i * step);
    }

    result = sum * step + result;
}


void parallel_integral (int N)
{
    double from = 0.1, to = 10;
    double from_tmp, to_tmp;

    std::vector<std::thread> threads;

    for (int i = 0; i < N; i++) 
    {
        from_tmp = from + (to - from) / N * i;
        to_tmp = from + (to - from) / N * (i + 1);

        if (i == N - 1)
            to_tmp = to;

        threads.push_back(std::thread(count_integral, from_tmp, to_tmp));
    }

    for (auto& thread : threads) 
    {
        thread.join();
    }
}


int main( )
{	
    int N = 1;
    parallel_integral(N);

    std::cout << result << std::endl;

    return 0;
}
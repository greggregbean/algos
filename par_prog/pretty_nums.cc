#include <iostream>
#include <atomic>
#include <thread>
#include <vector>

// The soulution is inspired by https://www.ega-math.narod.ru/Quant/Tickets.htm#A2 

unsigned long long count_total_num_s (unsigned sum_of_digits, 
                                      unsigned num_of_digits,
                                      unsigned base_of_digits)
{
    if (num_of_digits == 1) 
        return sum_of_digits < base_of_digits;

    unsigned long long total_num_s = 0;

    for (unsigned digit = 0; digit < base_of_digits; digit++)
    {
        total_num_s += count_total_num_s (sum_of_digits - digit, 
                                          num_of_digits - 1, 
                                          base_of_digits);
    }

    return total_num_s;
}

void increase_total_num (unsigned s, std::atomic<unsigned long long>& total_num)
{
    unsigned long long total_num_s = count_total_num_s (s, 6, 13);
    total_num += total_num_s * total_num_s;
}

int main()
{
    std::atomic<unsigned long long> total_num = 0;
    std::vector<std::thread> sum_threads; 

    for (unsigned s = 0; s <= 72; s++)
    {
        sum_threads.emplace_back(increase_total_num, s, std::ref(total_num));
    }

    for (auto& thread : sum_threads)
    {
        thread.join();
    }

    total_num = total_num * 13;

    std::cout << total_num << std::endl;
}
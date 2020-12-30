


#include "maxdefense.hh"
#include "timer.hh"
#include <bits/stdc++.h>
// helper function to print out the time for exhaustive with size of n
void time_testing_dynamic_programming(int n)
{
	auto all_armors = load_armor_database("armor.csv");
	auto filtered_armors = filter_armor_vector(*all_armors, 1, 5000, n);
	Timer timer;
	dynamic_max_defense(*filtered_armors, 10000);
	double elapsed = timer.elapsed();
	std::cout << "The n is " << n << std::endl;
	std::cout << "elapsed time = " << elapsed << " seconds for dynamic_max_defense"
						<< std::endl;

}
// helper function to print out the time for exhaustive with size of n
void time_testing_exhaustive(int n)
{
	auto all_armors = load_armor_database("armor.csv");
	auto filtered_armors = filter_armor_vector(*all_armors, 1, 5000, n);
	Timer timer;
	exhaustive_max_defense(*filtered_armors,10000);
	double elapsed = timer.elapsed();
	std::cout << "The n is " << n << std::endl;
	std::cout << "elapsed time = " << elapsed << " seconds for exhaustive_max_defense"
						<< std::endl;

}

int main()
{
	// Changing n to change the size for testing
	//testing for dynamic_max_defense
	time_testing_dynamic_programming(1);
	time_testing_dynamic_programming(2);
	time_testing_dynamic_programming(4);
	time_testing_dynamic_programming(8);
	time_testing_dynamic_programming(10);
	time_testing_dynamic_programming(100);
	time_testing_dynamic_programming(500);
	time_testing_dynamic_programming(1000);
	time_testing_dynamic_programming(2000);
	time_testing_dynamic_programming(4000);
	time_testing_dynamic_programming(10000);
	//testing for exhaustive_max_defense
	time_testing_exhaustive(1);
	time_testing_exhaustive(2);
	time_testing_exhaustive(4);
	time_testing_exhaustive(8);
	time_testing_exhaustive(10);
	time_testing_exhaustive(16);
	time_testing_exhaustive(20);
	time_testing_exhaustive(21);
	time_testing_exhaustive(22);
	time_testing_exhaustive(23);
	time_testing_exhaustive(24);
	return 0;
}

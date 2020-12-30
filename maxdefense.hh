///////////////////////////////////////////////////////////////////////////////
// maxdefense.hh
//
// Compute the set of armos that maximizes defense, within a gold budget,
// with the dynamic method or exhaustive search.
//
///////////////////////////////////////////////////////////////////////////////


#pragma once


#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <memory>
#include <queue>
#include <sstream>
#include <string>
#include <vector>


// One armor item available for purchase.
class ArmorItem
{
	//
	public:

		//
		ArmorItem
		(
			const std::string& description,
			size_t cost_gold,
			double defense_points
		)
			:
			_description(description),
			_cost_gold(cost_gold),
			_defense_points(defense_points)
		{
			assert(!description.empty());
			assert(cost_gold > 0);
		}

		//
		const std::string& description() const { return _description; }
		int cost() const { return _cost_gold; }
		double defense() const { return _defense_points; }

	//
	private:

		// Human-readable description of the armor, e.g. "new enchanted helmet". Must be non-empty.
		std::string _description;

		// Cost, in units of gold; Must be positive
		int _cost_gold;

		// Defense points; most be non-negative.
		double _defense_points;
};


// Alias for a vector of shared pointers to ArmorItem objects.
typedef std::vector<std::shared_ptr<ArmorItem>> ArmorVector;


// Load all the valid armor items from the CSV database
// Armor items that are missing fields, or have invalid values, are skipped.
// Returns nullptr on I/O error.
std::unique_ptr<ArmorVector> load_armor_database(const std::string& path)
{
	std::unique_ptr<ArmorVector> failure(nullptr);

	std::ifstream f(path);
	if (!f)
	{
		std::cout << "Failed to load armor database; Cannot open file: " << path << std::endl;
		return failure;
	}

	std::unique_ptr<ArmorVector> result(new ArmorVector);

	size_t line_number = 0;
	for (std::string line; std::getline(f, line); )
	{
		line_number++;

		// First line is a header row
		if ( line_number == 1 )
		{
			continue;
		}

		std::vector<std::string> fields;
		std::stringstream ss(line);

		for (std::string field; std::getline(ss, field, '^'); )
		{
			fields.push_back(field);
		}

		if (fields.size() != 3)
		{
			std::cout
				<< "Failed to load armor database: Invalid field count at line " << line_number << "; Want 3 but got " << fields.size() << std::endl
				<< "Line: " << line << std::endl
				;
			return failure;
		}

		std::string
			descr_field = fields[0],
			cost_gold_field = fields[1],
			defense_points_field = fields[2]
			;

		auto parse_dbl = [](const std::string& field, double& output)
		{
			std::stringstream ss(field);
			if ( ! ss )
			{
				return false;
			}

			ss >> output;

			return true;
		};

		std::string description(descr_field);
		double cost_gold, defense_points;
		if (
			parse_dbl(cost_gold_field, cost_gold)
			&& parse_dbl(defense_points_field, defense_points)
		)
		{
			result->push_back(
				std::shared_ptr<ArmorItem>(
					new ArmorItem(
						description,
						cost_gold,
						defense_points
					)
				)
			);
		}
	}

	f.close();

	return result;
}


// Convenience function to compute the total cost and defense in an ArmorVector.
// Provide the ArmorVector as the first argument
// The next two arguments will return the cost and defense back to the caller.
void sum_armor_vector
(
	const ArmorVector& armors,
	int& total_cost,
	double& total_defense
)
{
	total_cost = total_defense = 0;
	for (auto& armor : armors)
	{
		total_cost += armor->cost();
		total_defense += armor->defense();
	}
}


// Convenience function to print out each ArmorItem in an ArmorVector,
// followed by the total kilocalories and protein in it.
void print_armor_vector(const ArmorVector& armors)
{
	std::cout << "*** Armor Vector ***" << std::endl;

	if ( armors.size() == 0 )
	{
		std::cout << "[empty armor list]" << std::endl;
	}
	else
	{
		for (auto& armor : armors)
		{
			std::cout
				<< "Ye olde " << armor->description()
				<< " ==> "
				<< "Cost of " << armor->cost() << " gold"
				<< "; Defense points = " << armor->defense()
				<< std::endl
				;
		}

		int total_cost;
		double total_defense;
		sum_armor_vector(armors, total_cost, total_defense);
		std::cout
			<< "> Grand total cost: " << total_cost << " gold" << std::endl
			<< "> Grand total defense: " << total_defense
			<< std::endl
			;
	}
}


// Convenience function to print out a 2D cache, composed of an std::vector<std::vector<double>>
// For sanity, will refuse to print a cache that is too large.
// Hint: When running this program, you can redirect stdout to a file,
//	which may be easier to view and inspect than a terminal
void print_2d_cache(const std::vector<std::vector<double>>& cache)
{
	std::cout << "*** 2D Cache ***" << std::endl;

	if ( cache.size() == 0 )
	{
		std::cout << "[empty]" << std::endl;
	}

	else if ( cache.size() > 250 || cache[1].size() > 250 )
	{
		std::cout << "[too large]" << std::endl;
	}

	else
	{
		for ( const std::vector<double> row : cache)
		{
			for ( double value : row )
			{
				std::cout << std::setw(5) << value;
			}
			std::cout << std::endl;
		}
	}
}

// Filter the vector source, i.e. create and return a new ArmorVector
// containing the subset of the armor items in source that match given
// criteria.
// This is intended to:
//	1) filter out armor with zero or negative defense that are irrelevant to our optimization
//	2) limit the size of inputs to the exhaustive search algorithm since it will probably be slow.
//
// Each armor item that is included must have at minimum min_defense and at most max_defense.
//	(i.e., each included armor item's defense must be between min_defense and max_defense (inclusive).
//
// In addition, the the vector includes only the first total_size armor items that match these criteria.
std::unique_ptr<ArmorVector> filter_armor_vector
(
	const ArmorVector& source,
	double min_defense,
	double max_defense,
	int total_size
)
{
	// TODO: implement this function, then delete the return statement below
	std::unique_ptr<ArmorVector> tmp(new ArmorVector);
	// unsigned int make more efficient because index >=0
	for(size_t i = 0 ; i < source.size() && (*tmp).size() < (size_t)total_size ; i++)
	{
		if(source[i]->defense() >= min_defense && source[i]->defense() <= max_defense)
		{
			tmp->push_back(source[i]);
		}
	}
	return tmp;
}
//Max Function: return bigger number between 2 double numbers.
double max(double a, double b)
 {
	  return (a > b) ? a : b;
 }


// Compute the optimal set of armor items with a dynamic algorithm.
// Specifically, among the armor items that fit within a total_cost gold budget,
// choose the selection of armors whose defense is greatest.
// Repeat until no more armor items can be chosen, either because we've run out of armor items,
// or run out of gold.
std::unique_ptr<ArmorVector> dynamic_max_defense
(
	const ArmorVector& armors,
	int total_cost
)
{
	double const EPSILON = 0.001;
	std::unique_ptr<ArmorVector> result(new ArmorVector);
	std::unique_ptr<ArmorVector> todo(new ArmorVector(armors));
	int cost;
	std::vector<std::vector<double>> dp(todo->size() + 1, std::vector<double> (total_cost + 1));

	for( size_t i = 0 ; i <= todo->size() ; i++)
	{
		for ( cost = 0 ; cost <= total_cost; cost++)
		{
			if( i == 0 || cost == 0)
			{
				dp[i][cost] = 0;
			}
			else if ( todo->at(i - 1)->cost() <= cost )
			{

				dp[i][cost] = max((todo->at(i - 1)->defense() + dp[i - 1][cost - todo->at(i-1)->cost()]) ,
											dp[i - 1][cost]);
			}
			else
			{
				dp[i][cost]= dp[i - 1][cost];
			}
		}
	}
	double defense = dp[todo->size()][total_cost];
	cost = total_cost;
	for(size_t i = todo->size() ; i > 0 && defense > 0 ;i--)
	{
			if( std::abs(defense - dp[i - 1][cost]) < EPSILON)
			{
				continue;
			}
			if( defense != dp[i - 1][cost])
			{
				result->push_back(todo->at(i - 1));
				defense = defense - todo->at(i - 1)->defense();
				cost = cost - todo->at(i - 1)->cost();

			}

	}
	return result;
}


// Compute the optimal set of armor items with an exhaustive search algorithm.
// Specifically, among all subsets of armor items,
// return the subset whose gold cost fits within the total_cost budget,
// and whose total defense is greatest.
// To avoid overflow, the size of the armor items vector must be less than 64.
std::unique_ptr<ArmorVector> exhaustive_max_defense
(
	const ArmorVector& armors,
	double total_cost
)
{
	const int n = armors.size();
	assert(n < 64);
	std::unique_ptr<ArmorVector> best(new ArmorVector);
	std::unique_ptr<ArmorVector> candidate(new ArmorVector);
	int candidate_cost;
	double candidate_defense;
	double best_armor = 0;
	for(size_t i = 0; i <= (pow(2,n) - 1) ; i++)
	{
			candidate->clear();
			for( int j = 0 ; j < n ; j++ )
			{
				if( ((i >> j)  & 1) == 1)
				{
					candidate->push_back(armors[j]);
				}
			}

			sum_armor_vector( *candidate,candidate_cost,candidate_defense);
			if (candidate_cost <= total_cost )
			{
				if( best == NULL || candidate_defense > best_armor)
				{
					*best = *candidate;
					best_armor = candidate_defense;
				}
			}
	}

	// TODO: implement this function, then delete the return statement below
	return best;
}

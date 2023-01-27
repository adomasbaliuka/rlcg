#include <iostream>
#include <cassert>
#include <vector>

#include "rlcg.hpp"

/// Goal: get LCG that has ANY DESIRED PERIOD.
void test_fixed_period_lcg() {
    constexpr auto M = 20; // period of lcg. Can choose arbitrarily.

    // To get period = M, choose C to be relatively prime to M...
    constexpr auto C = 13;
    // ... and A s.t. (1.) (A-1) is divisible by all prime factors of M and (2.) (A-1) is divisible by 4 if M is.
    constexpr auto A = (2*2*5) + 1;

    constexpr auto seed = 42; // Seed of lcg. Can choose arbitrarily.
    rlcg::details::ReversibleLCG<M, A, C, 0> rng(seed);

    const int numtests = 5 * M;

    std::vector<unsigned int> forward;
    std::cout << "\nForward:\n";
    for(int i = 0; i<numtests; ++i){
        forward.push_back(rng.next());
        std::cout << forward.back() << std::endl;
        assert(forward.back() <= rng.max());
    }

    // test periodicity. Note: test only works correctly if forward.size() divides the period!
    for(std::size_t i = 0; i < forward.size(); ++i){
        assert(forward[i] == forward[(i + M) % forward.size()]);
    }

    std::cout << "\nBackwards:\n";
    for(int i = numtests - 2; i>=0; --i){
        unsigned int val = rng.prev();
        std::cout << val << std::endl;
        assert(val == forward[static_cast<unsigned int>(i)]);
    }

    std::vector<unsigned int> backward;
    std::cout << "\nBackwards:\n";
    for(int i = 0; i<numtests; ++i){
        backward.push_back(rng.prev());
        std::cout << backward.back() << std::endl;
    }
    std::cout << "\nForwards:\n";
    for(int i = numtests - 2; i>=0; --i){
        unsigned int val = rng.next();
        std::cout << val << std::endl;
        assert(val == backward[static_cast<unsigned int>(i)]);
    }
}


int main() {
    test_fixed_period_lcg();
    std::cout << "\n================== Original Example ==================\n";
    return 0;

	rlcg::ReversibleLCG rng(42);
	const int numtests = 10000;

	std::vector<unsigned int> forward;
	std::cout << "\nForward:\n";
	for(int i = 0; i<numtests; ++i){
		forward.push_back(rng.next());
		std::cout << forward.back() << std::endl;
		assert(forward.back() <= rng.max());
	}
	std::cout << "\nBackwards:\n";
	for(int i = numtests - 2; i>=0; --i){
		unsigned int val = rng.prev();
		std::cout << val << std::endl;
		assert(val == forward[static_cast<unsigned int>(i)]);
	}

	std::vector<unsigned int> backward;
	std::cout << "\nBackwards:\n";
	for(int i = 0; i<numtests; ++i){
		backward.push_back(rng.prev());
		std::cout << backward.back() << std::endl;
	}
	std::cout << "\nForwards:\n";
	for(int i = numtests - 2; i>=0; --i){
		unsigned int val = rng.next();
		std::cout << val << std::endl;
		assert(val == backward[static_cast<unsigned int>(i)]);
	}
	return 0;
}

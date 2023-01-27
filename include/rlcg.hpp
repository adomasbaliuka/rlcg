/*
    RLCG - Reversible Linear Congruential Generator
    Copyright (c) 2013 Johan Klokkhammer Helsing

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the authors be held liable for any damages
    arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

       1. The origin of this software must not be misrepresented; you must not
       claim that you wrote the original software. If you use this software
       in a product, an acknowledgment in the product documentation would be
       appreciated but is not required.

       2. Altered source versions must be plainly marked as such, and must not be
       misrepresented as being the original software.

       3. This notice may not be removed or altered from any source
       distribution.

    Johan Klokkhammer Helsing
    (johanhelsing@gmail.com)
*/

#ifndef RLCG_HPP
#define RLCG_HPP

#include <cstdint>
#include <tuple>

namespace rlcg {

namespace details {

template<class T>
constexpr bool isPowerOfTwo(T x){
    return (x & (x - 1)) == 0;
}

//constexpr implementation of euclids algorithm
/* Based on this recursive definition from wikipedia:
function extended_gcd(a, b)
  if b == 0
    return (1, 0)
  else
    (q, r) := divide (a, b)
    (s, t) := extended_gcd(b, r)
    return (t, s - q * t)
This assumes a "divide" procedure exists that returns a (quotient,remainder) pair (one could alternatively put q := a div b, and then r = a − b * q).
*/
constexpr std::tuple<uint64_t, uint64_t> extendedEuclid(uint64_t a, uint64_t b) {
    if (b == 0) {
        return std::make_tuple(1, 0);
    }
    else {
        uint64_t q = a / b; // integer division
        uint64_t r = a - b * q; // get the remainder
        auto [s, t] = extendedEuclid(b, r);
        return std::make_tuple(t, s - q * t);
    }
}


template <typename T>
constexpr inline T positive_modulo(T i, T n) {
    return (i % n + n) % n;
}

//modulus M, multiplicand A, increment C, least significant bits to discard D
template<uint64_t M = 1ull<<63ul, uint64_t A = 6364136223846793005, uint64_t C = 1442695040888963407, uint64_t D = 32>
class ReversibleLCG {
     static_assert(isPowerOfTwo(M) || D == 0,
             "M is not a power of two as it should be. I only tested behaviour for non-prime M when D = 0.");
    uint64_t x;
public:
    explicit ReversibleLCG(unsigned int seed) : x(seed){}
    // Hi 32-bits is the initial value, lo 32-bit is a random seed
    void set_state(uint64_t s) { x = s; }

    unsigned int next() {
        if constexpr (isPowerOfTwo(M)) {
            x = (A * x + C) & (M - 1);
        } else {
            x = (A * x + C) % M;
        }
        return x >> D;
    }

    unsigned int prev() {
        constexpr uint64_t ainverse = std::get<0>(extendedEuclid(A, M));
        if constexpr (isPowerOfTwo(M)) {
            x = ainverse * (x - C) & (M - 1);
        } else {
            constexpr auto minus_C_modM = positive_modulo(
                    -static_cast<long long>(C) , static_cast<long long>(M));
            x = ainverse * (x + minus_C_modM) % M;
        }

        return x >> D;
    }

    [[nodiscard]]
    constexpr unsigned int max() const {
        return (M - 1) >> D;
    }
};
} // end namespace details

using ReversibleLCG = details::ReversibleLCG<>;

} // end namespace rlcg


#endif // RLCG_HPP

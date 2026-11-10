/*
 * A340378  Number of 1-digits in the ternary representation of A048673(n).
 * https://oeis.org/A340378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A062756(A048673(n)), the number of 1's in the base-3
 *            expansion of (A003961(n)+1)/2
 * Range:     u8 n <= 16384, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9363 (2026-11-10)
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    unsigned __int128 prod = 1;
    if (n > 1) {
        uint64_t primes[64];
        uint64_t exps[64];
        size_t cnt = 0;
        factor_u64((uint64_t)n, primes, exps, &cnt);
        for (size_t i = 0; i < cnt; ++i) {
            uint64_t np = next_prime(primes[i]);
            for (uint64_t k = 0; k < exps[i]; ++k)
                prod = prod * (unsigned __int128)np;
        }
    }
    unsigned __int128 m = (prod + 1) / 2;
    unsigned c = 0;
    while (m != 0) {
        if ((m % 3) == 1)
            c++;
        m /= 3;
    }
    return (seq_t)c;
}

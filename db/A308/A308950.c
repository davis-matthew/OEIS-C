/*
 * A308950  Number of ways to write n as (p-1)/6 + 2^a*3^b, where p is a
 *          prime, and a and b are nonnegative integers.
 * https://oeis.org/A308950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (p,a,b) with p prime, a,b>=0 s.t. n ==
 *            (p-1)/6 + 2^a * 3^b
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"


seq_t a(idx_t n)
{
    uint64_t u_n = (uint64_t)n;
    seq_t count = 0;

    for (uint64_t pow2 = 1; pow2 <= u_n; pow2 <<= 1) {
        for (uint64_t pow3 = 1; pow3 <= u_n; pow3 *= 3) {
            __uint128_t product = (__uint128_t)pow2 * pow3;
            if (product > u_n)
                break;               /* all larger 3^b will also be too big */

            uint64_t term     = (uint64_t)product;
            uint64_t n_minus  = u_n - term;
            uint64_t p        = 6 * n_minus + 1;   /* candidate prime */

            if (is_prime((idx_t)p))
                count++;
        }
    }
    return count;
}

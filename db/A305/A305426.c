/*
 * A305426  Number of proper divisors of n of the form 2^k - 1 for k >= 1.
 * https://oeis.org/A305426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of proper divisors of n that are of the form 2^k -
 *            1 (k ≥ 1)
 * Range:     u8 n <= 9841, u16 n <= 9841, u32 n <= 9841, u64 n <= 9841,
 *            u128 n <= 9841, big any n
 * Helpers:   count_mersenne_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_mersenne_rec.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Treat n as an unsigned 64-bit integer for factorization. */
    uint64_t n64 = (uint64_t)n;

    /* Factorize n. */
    uint64_t primes[20];
    int     exps[20];
    int     num = 0;

    uint64_t temp = n64;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            primes[num] = p;
            exps[num]   = e;
            num++;
        }
    }
    if (temp > 1) {               /* Remaining prime factor. */
        primes[num] = temp;
        exps[num]   = 1;
        num++;
    }

    seq_t count = 0;
    /* Enumerate all divisors using the recursive helper. */
    count_mersenne_rec(primes, exps, 0, num, 1ULL, n64, &count);

    return count;
}

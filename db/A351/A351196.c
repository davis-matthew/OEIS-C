/*
 * A351196  Sum of the 8th powers of the primes dividing n.
 * https://oeis.org/A351196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p | n, p prime} p^8
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 16, u64 n <= 256,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, build_primes_upto, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A351196: the sum of the 8th powers of the
   distinct prime divisors of n. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/floor_sqrt.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    /* Keep a cache of primes up to the largest sqrt value seen so far. */
    static uint64_t *prime_arr = NULL;
    static size_t prime_cnt   = 0;
    static idx_t  prime_limit = 0;

    /* Determine the required prime limit (sqrt(n)). */
    idx_t sqrt_n = (idx_t)floor_sqrt((seq_t)n);

    /* Grow the prime cache if necessary. */
    if (sqrt_n > prime_limit) {
        free(prime_arr);                  /* free old cache */
        build_primes_upto(sqrt_n, &prime_arr, &prime_cnt);
        prime_limit = sqrt_n;
    }

    /* Factor n and accumulate the 8th powers of distinct primes. */
    uint64_t temp = (uint64_t)n;
    seq_t sum     = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_arr[i];
        if (p > temp / p) break;          /* p * p > temp */

        if (temp % p == 0) {
            sum += big_pow((seq_t)p, (seq_t)8);
            while (temp % p == 0) temp /= p;
        }
    }

    /* If a prime factor larger than sqrt(n) remains, add it. */
    if (temp > 1) {
        sum += big_pow((seq_t)temp, (seq_t)8);
    }

    return sum;
}

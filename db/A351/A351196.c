/*
 * A351196  Sum of the 8th powers of the primes dividing n.
 * https://oeis.org/A351196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p | n, p prime} p^8
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 16, u64 n <= 256,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 48 terms through n = 9579 (2026-11-01)
 * Helpers:   build_primes_upto, ipow, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A351196: the sum of the 8th powers of the
   distinct prime divisors of n. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow.h"
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

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
            sum += ipow((seq_t)p, (seq_t)8);
            while (temp % p == 0) temp /= p;
        }
    }

    /* If a prime factor larger than sqrt(n) remains, add it. */
    if (temp > 1) {
        sum += ipow((seq_t)temp, (seq_t)8);
    }

    return sum;
}

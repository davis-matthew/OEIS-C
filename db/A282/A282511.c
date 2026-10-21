/*
 * A282511  Numerator of the sum of the reciprocals of the first n nonprimes.
 * https://oeis.org/A282511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 23, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS sequence A282511.
   a(n) is the numerator of the reduced fraction representing
   the sum of the reciprocals of the first n nonprime integers. */

/*  Required helpers:
    - bool is_prime(idx_t x);
    - seq_t gcd(seq_t a, seq_t b);
    These are provided by the build in ../utils/is_prime.h and
    ../utils/gcd.h.  */
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/gcd.h"

/* Static cache large enough for all n used in tests (up to ~10000). */
#define MAXCACHE 20000
static seq_t num_cache[MAXCACHE];
static seq_t den_cache[MAXCACHE];
static idx_t cached_max = 0;   /* largest n already computed */
static idx_t next_candidate = 1; /* integer to test for nonprime */

/* Return numerator of a(n). */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined, but return 0 */

    /* If we already have this value, return it. */
    if (cached_max >= n)
        return num_cache[n - 1];

    /* Compute values up to n and fill the cache. */
    for (idx_t idx = cached_max + 1; idx <= n; ++idx) {
        /* Find the next nonprime integer. */
        while (is_prime(next_candidate))
            ++next_candidate;
        seq_t d = (seq_t)next_candidate;

        if (cached_max == 0 && idx == 1) {
            /* First term: 1/1. */
            num_cache[0] = (seq_t)1;
            den_cache[0] = (seq_t)1;
        } else {
            seq_t prev_num = num_cache[idx - 2];
            seq_t prev_den = den_cache[idx - 2];
            /* Add 1/d to the previous fraction: (prev_num/prev_den) + 1/d. */
            seq_t new_num = prev_num * d + prev_den;
            seq_t new_den = prev_den * d;
            /* Reduce the fraction. */
            seq_t g = gcd(new_num, new_den);
            new_num /= g;
            new_den /= g;
            num_cache[idx - 1] = new_num;
            den_cache[idx - 1] = new_den;
        }

        /* Move to the next integer for the following iteration. */
        ++next_candidate;
    }

    cached_max = n;
    return num_cache[n - 1];
}

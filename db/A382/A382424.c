/*
 * A382424  The number of exponents in the prime factorization of n-th
 *          biquadratefree number that are equal to 3.
 * https://oeis.org/A382424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence gives, for the n‑th biquadratefree integer, the number of
   exponents that are exactly 3 in its prime factorisation.
   An integer is biquadratefree if no prime factor has exponent ≥4.
   This implementation incrementally generates biquadratefree numbers,
   counts the exponents equal to 3 and memoises the results. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

static seq_t *cache = NULL;         /* a[0] .. a[sz-1] */
static idx_t cache_size = 0;        /* number of values stored */
static uint64_t next_candidate = 1; /* next integer to test for biquadratefree */

/* count and store a(n) if n not yet computed */
static void ensure_n(idx_t n)
{
    /* grow cache array if necessary */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * n);
        cache_size = 0;
    } else if (cache_size < n) {
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * n);
    }

    /* generate entries until we have n values */
    while (cache_size < n) {
        uint64_t prime[16];
        uint64_t exp[16];
        size_t fcnt;
        /* factor the candidate */
        factor_u64(next_candidate, prime, exp, &fcnt);

        bool is_bq = true;
        seq_t val = 0;
        for (size_t i = 0; i < fcnt; ++i) {
            if (exp[i] >= 4) {
                is_bq = false;
                break;
            }
            if (exp[i] == 3) {
                val += 1;
            }
        }
        if (is_bq) {
            cache[cache_size++] = val;
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for index 0, but return 0 for safety */
    ensure_n(n);
    return cache[n - 1];
}

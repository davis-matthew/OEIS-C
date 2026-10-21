/*
 * A283764  a(0) = 0; a(1) = 1; a(2*n) = sigma(a(n)), a(2*n+1) =
 *          sigma(a(n)+a(n+1)).
 * https://oeis.org/A283764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(1) = 1; a(2*n) = sigma(a(n)); a(2*n+1) = sigma(a(n)
 *            + a(n+1)); where sigma(x) is the sum of positive divisors of x
 * Range:     u8 n <= 70, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"

static seq_t *cache = NULL;
static bool *computed = NULL;
static size_t cache_cap = 0;

/* Ensure that cache and computed arrays have size at least n+1. */
static void ensure_arrays(idx_t n)
{
    if (n + 1 > cache_cap) {
        size_t new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        computed = (bool *)realloc(computed, new_cap * sizeof(bool));
        if (!cache || !computed) exit(1);          /* abort on allocation failure */
        /* initialise new slots */
        for (size_t i = cache_cap; i < new_cap; ++i) {
            computed[i] = false;
        }
        cache_cap = new_cap;
    }
}

static seq_t sigma_seq(seq_t x)
{
    /* work only up to 64 bits; in practice the sequence never exceeds this */
    return (seq_t)sigma_of((idx_t)x);
}

/* recursive computation with memoisation */
seq_t a(idx_t n)
{
    ensure_arrays(n);
    if (computed[n]) return cache[n];

    seq_t result;
    if (n == 0) {
        result = 0;
    } else if (n == 1) {
        result = 1;
    } else if ((n & 1) == 0) {
        /* n is even: a(n) = sigma(a(n/2)) */
        idx_t m = n >> 1;
        result = sigma_seq(a(m));
    } else {
        /* n is odd: a(n) = sigma(a((n-1)/2) + a((n+1)/2)) */
        idx_t m = (n - 1) >> 1;
        seq_t sum = a(m) + a(m + 1);
        result = sigma_seq(sum);
    }
    cache[n] = result;
    computed[n] = true;
    return result;
}

/*
 * A330908  a(n+1) = a(n) + (number of divisors of a(n) that are not divisors
 *          of other divisors of a(n)) for n>1; a(1)=1.
 * https://oeis.org/A330908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + (ω(a(n-1)) + 1), a(1) = 1 ω(m) = number of
 *            distinct prime factors of m
 * Range:     u8 n <= 88, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/omega.h"

/* Static cache for computed values.  a(1) is stored at seq_cache[0]. */
static seq_t *seq_cache = NULL;
static idx_t seq_cache_size  = 0;   /* number of terms stored */
static idx_t seq_cache_cap   = 0;   /* allocated capacity */

/* Ensure cache holds at least n terms. */
static void ensure_cache_size(idx_t n)
{
    if (seq_cache_size >= n) return;

    if (seq_cache_cap < n) {
        idx_t new_cap = seq_cache_cap == 0 ? 16 : seq_cache_cap;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_cache = tmp;
        seq_cache_cap = new_cap;
    }

    /* If cache empty, initialise a(1) */
    if (seq_cache_size == 0) {
        seq_cache[0] = 1;
        seq_cache_size = 1;
    }

    for (idx_t i = seq_cache_size + 1; i <= n; ++i) {
        seq_t prev = seq_cache[i - 2];      /* a(i-1) */
        seq_t inc  = omega(prev) + 1;
        seq_cache[i - 1] = prev + inc;      /* a(i) */
    }
    seq_cache_size = n;
}

/* External entry point required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out of range, not used */
    ensure_cache_size(n);
    return seq_cache[n - 1];
}

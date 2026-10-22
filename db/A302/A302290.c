/*
 * A302290  a(n) is the 2-norm of denominators of two-variable polynomials of
 *          degree n which are integer-valued.
 * https://oeis.org/A302290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the recurrence given in the OEIS entry for
 *            A302290. No closed form is known, so the function computes the
 *            value recursively with memoisation. The cache is implemented as
 *            an array of seq_t objects allocated with malloc/realloc. The
 *            array grows geometrically to accommodate arbitr...
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/pow_uint.h"

static seq_t *cache   = NULL;   /* cache[n] = a(n) */
static unsigned char *seen = NULL; /* seen[n] == 1 iff cache[n] is valid */
static size_t cache_size = 0;   /* current number of elements */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n < cache_size) return;
    size_t new_size = cache_size ? cache_size << 1 : 1;
    while (new_size <= (size_t)n) new_size <<= 1;
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    seen  = (unsigned char *)realloc(seen, new_size);
    for (size_t i = cache_size; i < new_size; ++i) seen[i] = 0;
    cache_size = new_size;
}

/* Recursive computation of a(n) with memoisation. */
static seq_t rec(idx_t n)
{
    ensure_capacity(n);
    if (seen[n]) return cache[n];

    /* Base case */
    if (n == 0) {
        cache[0] = (seq_t)1;
        seen[0]  = 1;
        return cache[0];
    }

    /* Determine k such that 2^k - k - 1 <= n < 2^(k+1)-k-2. */
    idx_t k = 0;
    idx_t p = 1;           /* 2^k */
    while (1) {
        idx_t T = p - k - 1;
        if (T > n) break;
        ++k;
        p <<= 1;
    }
    --k;                     /* correct to largest k with 2^k-k-1 <= n */
    p = (idx_t)1 << k;
    idx_t T = p - k - 1;     /* 2^k - k - 1 */
    idx_t U = p - 1;         /* 2^k - 1 */
    idx_t V = 2 * T;         /* 2*(2^k - k - 1) */

    seq_t result;

    if (n == T) {
        /* a(n) = 2^(k-1) */
        result = pow_uint((uint64_t)2, k - 1);
    } else if (n > T && n < U) {
        /* a(n) = a(2*T - n) */
        idx_t m = 2 * T - n;
        result = rec(m);
    } else if (n >= U && n <= V) {
        /* a(n) = a(2*T - n) + 2*a(n - 2^k + 1) */
        idx_t m   = 2 * T - n;
        idx_t diff= n - (p - 1);
        seq_t left  = rec(m);
        seq_t right = rec(diff);
        result = left + right * (seq_t)2;
    } else {
        /* V < n < 2^(k+1) - k - 2  →  a(n) = 2*a(n - 2^k + 1) */
        idx_t diff= n - (p - 1);
        seq_t val = rec(diff);
        result = val * (seq_t)2;
    }

    cache[n] = result;
    seen[n] = 1;
    return result;
}

/* Public entry point used by the test harness. */
seq_t a(idx_t n)
{
    return rec(n);
}

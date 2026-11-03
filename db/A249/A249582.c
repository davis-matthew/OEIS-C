/*
 * A249582  Norm of the complex coefficients in 1/(1 - x + (1-2*I)*x^2).
 * https://oeis.org/A249582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 28 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A249582: Norm of the complex coefficients in 1/(1 - x + (1-2*I)*x^2)
 *
 * Generating function:
 *   A(x) = (1-5*x^2)/(1 - x - 8*x^2 - 5*x^3 + 25*x^4)
 *
 * Recurrence (for n ≥ 4):
 *   a(n) = a(n-1) + 8*a(n-2) + 5*a(n-3) – 25*a(n-4)
 *   with initial values
 *   a(0)=1, a(1)=1, a(2)=4, a(3)=17.
 *
 * The implementation caches computed terms so that any call
 * a(n) runs in O(n) time relative to the highest n previously
 * requested.  It works with integer seq_t (uint8_t…uint64_t,
 * unsigned __int128) as well as with an arbitrary‑precision
 * class for seq_t (in C++).  Multiplication by small constants
 * is performed with an intermediate width wide enough to
 * avoid overflow on integer builds.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

#ifdef __cplusplus
/* In C++ the seq_t type is a class that supports the usual
 * arithmetic operators.  The following helper simply wraps
 * multiplication so that the same code works for both C and C++. */
static inline seq_t mul_uint(seq_t x, unsigned int m) { return x * m; }
#else
#if defined(__SIZEOF_INT128__)  /* GCC/Clang provide __uint128_t */
static inline seq_t mul_uint(seq_t x, unsigned int m) {
    return (seq_t)(((__uint128_t)x) * m);
}
#else
/* If 128‑bit type is not available, we rely on the property that
 * for this recurrence the 64‑bit product fits into seq_t whenever
 * the final result does.  Hence the plain multiplication is safe
 * for values that fit in seq_t. */
static inline seq_t mul_uint(seq_t x, unsigned int m) { return x * m; }
#endif
#endif

/* Static cache for memoisation */
static seq_t *cache = NULL;   /* array holding computed values */
static size_t  cap   = 0;      /* allocated capacity of cache    */
static idx_t   max_n = (idx_t)(-1);  /* last computed index          */

/* Return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    /* Initialise cache if first call or grow if necessary. */
    size_t needed = n + 1;
    if (needed < 4) needed = 4;               /* need space for a0..a3 */
    if (cache == NULL) {
        grow_seq(&cache, &cap, needed);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)17;
        max_n   = 3;
    } else if (cap <= n) {
        grow_seq(&cache, &cap, needed);
    }

    /* If we already have the term, return it immediately. */
    if (n <= max_n) return cache[n];

    /* Compute terms up to n using the recurrence. */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        seq_t t1 = cache[k-1];
        seq_t t2 = mul_uint(cache[k-2], 8);
        seq_t t3 = mul_uint(cache[k-3], 5);
        seq_t t4 = mul_uint(cache[k-4], 25);
        cache[k] = t1 + t2 + t3 - t4;
    }
    max_n = n;
    return cache[n];
}

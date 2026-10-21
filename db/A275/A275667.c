/*
 * A275667  Number of ON cells after n generations in a 2-dimensional
 *          "Odd-Rule" cellular automaton on triangular tiling.
 * https://oeis.org/A275667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 508, u32 n <= 9336, u64 n <= 9336,
 *            u128 n <= 9336, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A275667
   Recurrence:
       a(0) = 1
       a(2*t+1) = 3*a(t)
       a(4*t)   = a(2*t)
       a(8*t+2) = 3*a(4*t) + 4*a(2*t)
       a(8*t+6) = 3*a(4*t+2) + 4*a(2*t)
   This implementation uses memoisation.  The cache is an array of
   seq_t values, initialised to zero.  A value of zero indicates
   “not yet computed”; all terms of the sequence are positive, so the
   sentinel is safe.  Allocation works for both C (malloc/free) and
   C++ (new/delete) and for arbitrary‑precision seq_t types.  The
   routine is O(log n) per call and suitable for n up to at least 10 000. */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;      /* cache of computed terms */
static size_t cache_cap = 0;     /* current capacity of cache */

/* Extend the cache so that indices up to n are valid. */
static void ensure_cache(idx_t n)
{
    if (cache_cap > (size_t)n) return;          /* already large enough */

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    /* Allocate new array of seq_t objects. */
    seq_t *new_cache;
#ifdef __cplusplus
    new_cache = new seq_t[new_cap];
#else
    new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
    if (!new_cache) exit(EXIT_FAILURE);

    /* Initialise all slots to the sentinel value zero. */
    for (size_t i = 0; i < new_cap; ++i) new_cache[i] = 0;

    /* Copy existing values and free old storage. */
    if (cache) {
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
    }

    cache     = new_cache;
    cache_cap = new_cap;
}

/* Recursive helper that returns a(n), using memoisation. */
static seq_t rec(idx_t n)
{
    ensure_cache(n);

    if (cache[n] != 0)           /* already computed */
        return cache[n];

    seq_t result;
    if (n == 0) {
        result = 1;
    } else if ((n & 1) == 1) {    /* n is odd:  a(2*t+1) = 3*a(t) */
        idx_t t = (n - 1) >> 1;
        result = 3 * rec(t);
    } else if ((n & 3) == 0) {    /* n ≡ 0 (mod 4): a(4*t) = a(2*t) */
        result = rec(n >> 1);
    } else if ((n & 7) == 2) {    /* n ≡ 2 (mod 8): a(8*t+2) */
        idx_t t   = n >> 3;                /* t = floor(n/8)  */
        seq_t a4t = rec(t << 2);           /* a(4*t)   */
        seq_t a2t = rec(t << 1);           /* a(2*t)   */
        result = 3 * a4t + 4 * a2t;
    } else if ((n & 7) == 6) {    /* n ≡ 6 (mod 8): a(8*t+6) */
        idx_t t    = n >> 3;                     /* t = floor(n/8)  */
        seq_t a4t2 = rec((t << 2) + 2);          /* a(4*t+2) */
        seq_t a2t  = rec(t << 1);                /* a(2*t)   */
        result = 3 * a4t2 + 4 * a2t;
    } else {                      /* n ≡ 4 (mod 8) falls into the 4*t case */
        result = rec(n >> 1);
    }

    cache[n] = result;            /* store for future use */
    return result;
}

/* Public entry point required by the build system. */
seq_t a(idx_t n)
{
    return rec(n);
}

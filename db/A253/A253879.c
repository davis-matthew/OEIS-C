/*
 * A253879  Indices of centered heptagonal numbers (A069099) which are also
 *          triangular numbers (A000217).
 * https://oeis.org/A253879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*a(n-1) - 17*a(n-2) + a(n-3) (n≥4) with a(1)=1, a(2)=9,
 *            a(3)=136
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A253879.
   The function is memoized so that calls in any order are fast. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0 for the out‑of‑range index 0 (not used in the OEIS offset). */
    if (n == 0)
        return (seq_t)0;

    /* Static cache */
    static seq_t *cache = NULL;
    static size_t sz   = 0;   /* number of terms stored (a[1] at cache[0]) */
    static size_t cap  = 0;   /* allocated capacity */

    /* Initialise cache with first three terms */
    if (cache == NULL) {
        cap = 4;                          /* enough for a(1)..a(4) */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* out of memory, fail gracefully */
        cache[0] = (seq_t)1;              /* a(1) */
        if (cap > 1) cache[1] = (seq_t)9; /* a(2) */
        if (cap > 2) cache[2] = (seq_t)136;/* a(3) */
        sz = 3;
    }

    /* Compute up to the requested term */
    while (sz < (size_t)n) {
        seq_t new_val;
        size_t k = sz + 1; /* next index (1‑based) */

        if (k == 1)
            new_val = (seq_t)1;
        else if (k == 2)
            new_val = (seq_t)9;
        else if (k == 3)
            new_val = (seq_t)136;
        else {
            /* Use the recurrence a(k) = 17*a(k-1) - 17*a(k-2) + a(k-3) */
            seq_t a1 = cache[k - 2]; /* a(k-1) */
            seq_t a2 = cache[k - 3]; /* a(k-2) */
            seq_t a3 = cache[k - 4]; /* a(k-3) */
            new_val = (seq_t)17 * a1 - (seq_t)17 * a2 + a3;
        }

        /* Grow cache if needed */
        if (sz >= cap) {
            size_t new_cap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return new_val; /* allocation failed, return result */
            cache = tmp;
            cap = new_cap;
        }

        cache[sz] = new_val;
        ++sz;
    }

    return cache[n - 1];
}

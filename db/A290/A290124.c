/*
 * A290124  a(n) = a(n-1) + 12*a(n-2) with a(1) = 1 and a(2) = 2.
 * https://oeis.org/A290124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 12*a(n-2) with a(1)=1, a(2)=2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ------------------------------------------------------------------ */
/* Cached sequence storage.                                           */
/* ------------------------------------------------------------------ */
static seq_t *cache = NULL;       /* array of terms, indices start at 1   */
static size_t cap   = 0;          /* allocated length of the array       */
static size_t last  = 0;          /* largest computed index              */

/* ------------------------------------------------------------------ */
/* Public function: compute a(n).                                     */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Initial allocation on first call.                           */
    if (cap == 0) {
        /* Allocate for indices 0..2 (size 3).  Index 0 unused.     */
        cap   = 3;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;          /* out of memory – return 0 */
        cache[0] = 0;                  /* unused */
        cache[1] = 1;                  /* a(1) */
        cache[2] = 2;                  /* a(2) */
        last = 2;                      /* largest computed index */
    }

    /* If n is already computed, return it.                         */
    if ((idx_t)n <= (idx_t)last) {
        return cache[n];
    }

    /* Grow the array if necessary.                                 */
    if ((idx_t)n >= (idx_t)cap) {
        size_t new_cap = cap;
        /* Double until we can hold index n.                         */
        while (new_cap <= (size_t)n) {
            new_cap <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;              /* realloc failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute terms from last+1 up to n.                           */
    seq_t a0 = cache[1];                /* a(1) */
    seq_t a1 = cache[2];                /* a(2) */
    /* If we have computed more than 2 terms, recover last two.    */
    if (last >= 3) {
        a0 = cache[last-1];
        a1 = cache[last];
    }
    for (idx_t i = (idx_t)last + 1; i <= n; ++i) {
        seq_t a2 = a1 + (seq_t)12 * a0;  /* a(i) = a(i-1) + 12*a(i-2) */
        cache[i] = a2;
        a0 = a1;
        a1 = a2;
    }

    last = (size_t)n;
    return cache[n];
}

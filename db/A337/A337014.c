/*
 * A337014  a(1) = 0 and for n > 1, a(n+1) = (k(n) - a(n))*(-1)^(n+1) where
 *          k(n) is the number of terms equal to a(n) among the first n terms.
 * https://oeis.org/A337014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(1)=0; for n>1, a(n+1) = (k(n)-a(n))*(-1)^(n+1), where k(n) = #
 *            of terms equal to a(n) among first n terms
 * Range:     i8 n <= 4815, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t types are supplied by the build environment. */
extern seq_t *seq_alloc(size_t);
extern void seq_free(seq_t *);

/* Memoized computation of the sequence, works for any order of calls. */
seq_t a(idx_t n)
{
    /* static state – memoization of computed terms */
    static seq_t *arr = NULL;   /* 1‑based array of values */
    static idx_t cap = 0;       /* allocated capacity (≥ size) */
    static idx_t max_n = 0;     /* highest index computed so far */

    /* validate input */
    if (n < 1) {
        /* undefined for n<1; return 0 as a fallback */
        return (seq_t)0;
    }

    /* extend storage if necessary */
    if (cap < n + 1) {
        size_t new_cap = (cap < 8) ? 8 : cap * 2;
        while (new_cap < n + 1) new_cap *= 2;
        arr = (seq_t *)realloc(arr, new_cap * sizeof(seq_t));
        if (!arr) abort();     /* out of memory */
        cap = (idx_t)new_cap;
    }

    /* initialise first element on first use */
    if (max_n == 0) {
        arr[1] = (seq_t)0;
        max_n   = 1;
    }

    /* compute terms up to requested n */
    for (idx_t idx = max_n + 1; idx <= n; ++idx) {
        /* previous value a(idx-1) */
        seq_t a_prev = arr[idx - 1];

        /* count occurrences of a_prev among first idx-1 terms */
        idx_t k = 0;
        for (idx_t j = 1; j < idx; ++j)
            if (arr[j] == a_prev)
                ++k;

        /* sign = (-1)^idx : +1 if idx even, -1 if odd */
        seq_t sign = (idx & 1) ? (seq_t)-1 : (seq_t)1;

        /* compute a(idx) */
        arr[idx] = ( (seq_t)k - a_prev ) * sign;
    }
    max_n = n;

    return arr[n];
}

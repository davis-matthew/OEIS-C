/*
 * A305687  Number of nX3 0..1 arrays with every element unequal to 1, 2, 3,
 *          4, 5 or 6 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A305687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 8*a(n-2) + 12*a(n-3) + 11*a(n-4) - a(n-5) +
 *            a(n-6) + a(n-7) - a(n-8) for n ≥ 9, with initial values: a(1)=1,
 *            a(2)=25, a(3)=152, a(4)=1151, a(5)=8415, a(6)=61796,
 *            a(7)=453553, a(8)=3329201
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoizes results so that calls can be made in any order. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values for n = 1 … 8 */
    static const seq_t init[8] = {
        1ULL,
        25ULL,
        152ULL,
        1151ULL,
        8415ULL,
        61796ULL,
        453553ULL,
        3329201ULL
    };

    /* Cached terms up to the largest n previously computed. */
    static seq_t *terms = NULL;
    static idx_t stored = 0;   /* number of terms stored (corresponds to max n computed) */

    /* Return early for the first eight terms. */
    if (n <= 8)
        return init[n - 1];

    /* Allocate cache for the first eight terms on first use. */
    if (stored == 0) {
        terms = (seq_t*)malloc(8 * sizeof(seq_t));
        for (idx_t i = 0; i < 8; ++i)
            terms[i] = init[i];
        stored = 8;
    }

    /* Extend the cache if necessary and compute missing terms using the recurrence. */
    if (n > stored) {
        /* Grow the array to hold up to n terms. */
        seq_t *tmp = (seq_t*)realloc(terms, (size_t)n * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure – fallback to computing on the fly (unlikely). */
            /* In a real system we might abort or signal an error; here we simply return 0. */
            return (seq_t)0;
        }
        terms = tmp;

        /* Compute terms from stored+1 up to n inclusive. */
        for (idx_t k = stored + 1; k <= n; ++k) {
            /* indices in the cache: terms[0] = a(1), terms[1] = a(2), … */
            seq_t val =
              6 * terms[k - 2]   /* a(k-1) */
            + 8 * terms[k - 3]   /* a(k-2) */
            +12 * terms[k - 4]   /* a(k-3) */
            +11 * terms[k - 5]   /* a(k-4) */
            -  terms[k - 6]     /* a(k-5) */
            +  terms[k - 7]     /* a(k-6) */
            +  terms[k - 8]     /* a(k-7) */
            -  terms[k - 9];    /* a(k-8) */
            terms[k - 1] = val;
        }
        stored = n;
    }

    return terms[n - 1];
}

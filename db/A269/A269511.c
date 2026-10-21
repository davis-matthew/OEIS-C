/*
 * A269511  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 342", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A269511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 56, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)+2*a(n-2)-2*a(n-3)-2*a(n-6)+2*a(n-7)+a(n-8)-a(n-9)
   with initial values a(0)=1, a(1)=6, a(2)=14, a(3)=38, a(4)=55,
   a(5)=108, a(6)=140, a(7)=236, a(8)=285
   (OEIS A269511) */

#include <stdlib.h>

static seq_t *seq_cache = NULL;
static idx_t   seq_len   = 0;   /* number of values that have been computed */
static idx_t   seq_cap   = 0;   /* current allocated capacity */

/* First nine terms of the sequence (indices 0..8) */
static const seq_t seq_init[9] = {
    (seq_t)1,  (seq_t)6,  (seq_t)14, (seq_t)38, (seq_t)55,
    (seq_t)108, (seq_t)140, (seq_t)236, (seq_t)285
};

seq_t a(idx_t n)
{
    /* Initialise the cache on the first call */
    if (seq_cache == NULL) {
        seq_cap = 9;
        seq_cache = (seq_t *)malloc(sizeof(seq_t) * seq_cap);
        if (seq_cache == NULL)
            return (seq_t)0;   /* allocation failure – unlikely */
        for (idx_t i = 0; i < 9; ++i)
            seq_cache[i] = seq_init[i];
        seq_len = 9;
    }

    /* Return value if already computed */
    if (n < seq_len)
        return seq_cache[n];

    /* Grow the cache until it can contain index n */
    if (seq_cap <= n) {
        idx_t new_cap = seq_cap * 2;
        if (new_cap == 0)
            new_cap = 1;
        if (new_cap < n + 1)
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL)
            return (seq_t)0;   /* allocation failure – unlikely */
        seq_cache = tmp;
        seq_cap = new_cap;
    }

    /* Compute new terms using the recurrence */
    for (idx_t i = seq_len; i <= n; ++i) {
        /* Guarantee that the computation does not underflow: 
           group positive and negative contributions separately. */
        seq_t pos = seq_cache[i-1]
                  + (seq_t)2 * seq_cache[i-2]
                  + (seq_t)2 * seq_cache[i-7]
                  + seq_cache[i-8];

        seq_t neg = (seq_t)2 * seq_cache[i-3]
                  + (seq_t)2 * seq_cache[i-6]
                  + seq_cache[i-9];

        seq_cache[i] = pos - neg;
    }

    seq_len = n + 1;
    return seq_cache[n];
}

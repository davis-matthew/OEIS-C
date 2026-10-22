/*
 * A375989  Expansion of e.g.f. (1 + 3 * log(1 - x))^(5/3).
 * https://oeis.org/A375989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=0..n} (∏_{j=0}^{k-1} (3*j-5)) * |Stirling1(n,k)|
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (The product for k=0 is considered to be 1.) */

#include <stdlib.h>
#include <stdbool.h>

/* Public entry point */
seq_t a(idx_t n)
{
    /* Static data for memoisation */
    static bool initialised = false;
    static seq_t *cache   = NULL;       /* memoised a values */
    static size_t cache_cap = 0, cache_len = 0;
    static seq_t *P      = NULL;       /* product terms P[k] */
    static size_t P_cap   = 0;
    static seq_t *prev_row = NULL;     /* Stirling numbers for current n */
    static size_t prev_len  = 0;
    static seq_t *next_row = NULL;     /* temporary buffer for new row */
    static size_t next_cap  = 0;
    static idx_t current_n = 0;        /* highest n computed */

    /* Initialise once */
    if (!initialised)
    {
        /* Allocation for n = 0 */
        cache     = (seq_t *)malloc(sizeof(seq_t));
        cache[0]  = (seq_t)1;
        cache_len = cache_cap = 1;

        P         = (seq_t *)malloc(sizeof(seq_t));
        P[0]      = (seq_t)1;
        P_cap     = 1;

        prev_row  = (seq_t *)malloc(sizeof(seq_t));
        prev_row[0] = (seq_t)1;         /* Stirling1(0,0) = 1 */
        prev_len  = 1;

        current_n = 0;
        next_row  = NULL;
        next_cap  = 0;

        initialised = true;
    }

    /* Return cached value if already computed */
    if (n < cache_len)
        return cache[n];

    /* Compute up to the requested n */
    while (current_n < n)
    {
        idx_t new_n = current_n + 1;

        /* Resize cache if necessary (target size is full request n+1) */
        if ((size_t)(n + 1) > cache_cap)
        {
            size_t new_cap = cache_cap;
            while ((size_t)(n + 1) > new_cap)
                new_cap = new_cap ? new_cap * 2 : 1;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        /* Resize P array if necessary */
        if ((size_t)(new_n + 1) > P_cap)
        {
            size_t new_cap = P_cap;
            while ((size_t)(new_n + 1) > new_cap)
                new_cap = new_cap ? new_cap * 2 : 1;
            P = (seq_t *)realloc(P, new_cap * sizeof(seq_t));
            P_cap = new_cap;
        }

        /* Correctly compute the factor as signed */
        seq_t factor = (seq_t)(3 * (seq_t)(new_n - 1) - 5);
        P[new_n] = P[new_n - 1] * factor;

        /* Resize next_row buffer if necessary */
        if ((size_t)(new_n + 1) > next_cap)
        {
            next_row = (seq_t *)realloc(next_row, (new_n + 1) * sizeof(seq_t));
            next_cap = new_n + 1;
        }

        /* Stirling numbers of the first kind |s(n,k)| (unsigned) */
        next_row[0] = (seq_t)0;          /* s(new_n,0) = 0 for n>0 */
        seq_t mult = (seq_t)(new_n - 1);
        for (idx_t k = 1; k < new_n; ++k)
        {
            /* c(new_n,k) = c(prev,k-1) + (new_n-1)*c(prev,k) */
            next_row[k] = prev_row[k - 1] + mult * prev_row[k];
        }
        next_row[new_n] = (seq_t)1;      /* c(new_n,new_n) = 1 */

        /* Sum over k to obtain a(new_n) */
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= new_n; ++k)
            sum += P[k] * next_row[k];

        /* Store the result in cache */
        cache[new_n] = sum;
        if ((size_t)(new_n + 1) > cache_len)
            cache_len = new_n + 1;

        /* Prepare for next iteration */
        free(prev_row);
        prev_row = next_row;
        prev_len = new_n + 1;
        next_row = NULL;
        next_cap = 0;
        current_n = new_n;
    }

    return cache[n];
}

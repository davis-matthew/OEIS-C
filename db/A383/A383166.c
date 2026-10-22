/*
 * A383166  Expansion of e.g.f. log(1 + (exp(2*x) - 1)/2)^3 / 6.
 * https://oeis.org/A383166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k = 3..n} 2^(n-k) * Stirling2(n,k) * Stirling1(k,3)
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 15, i64 n <= 24, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Stirling1(k,3) = (-1)^(k-3) * unsigned_stirling1(k,3) */

#include <stdlib.h>

/* External types defined by the build system */
seq_t a(idx_t n);

/* Function implementation */
seq_t a(idx_t n)
{
    /* Static caches */
    static idx_t max_comp = 0;
    static seq_t *a_cache   = NULL;
    static seq_t *pow2      = NULL;
    static seq_t *s1_arr    = NULL;
    static seq_t *s2_arr    = NULL;
    static seq_t *s3_arr    = NULL;
    static seq_t *s2_prev   = NULL;
    static seq_t *s2_curr   = NULL;

    /* Initialise the caches on the first call (even for n==0) */
    if (max_comp == 0 && a_cache == NULL)
    {
        size_t initSize = 1;
        a_cache   = (seq_t*)realloc(a_cache,     initSize * sizeof(seq_t));
        pow2      = (seq_t*)realloc(pow2,        initSize * sizeof(seq_t));
        s1_arr    = (seq_t*)realloc(s1_arr,      initSize * sizeof(seq_t));
        s2_arr    = (seq_t*)realloc(s2_arr,      initSize * sizeof(seq_t));
        s3_arr    = (seq_t*)realloc(s3_arr,      initSize * sizeof(seq_t));
        s2_prev   = (seq_t*)realloc(s2_prev,     initSize * sizeof(seq_t));
        s2_curr   = (seq_t*)realloc(s2_curr,     initSize * sizeof(seq_t));

        a_cache[0]  = 0;
        pow2[0]     = 1;
        s1_arr[0]   = 0;
        s2_arr[0]   = 0;
        s3_arr[0]   = 0;
        s2_prev[0]  = 1; /* S2(0,0) = 1 */
    }

    /* Expand the caches if the requested n is larger than what we have */
    if (n > max_comp)
    {
        size_t newSize = (size_t)(n + 1);
        a_cache   = (seq_t*)realloc(a_cache,   newSize * sizeof(seq_t));
        pow2      = (seq_t*)realloc(pow2,      newSize * sizeof(seq_t));
        s1_arr    = (seq_t*)realloc(s1_arr,    newSize * sizeof(seq_t));
        s2_arr    = (seq_t*)realloc(s2_arr,    newSize * sizeof(seq_t));
        s3_arr    = (seq_t*)realloc(s3_arr,    newSize * sizeof(seq_t));
        s2_prev   = (seq_t*)realloc(s2_prev,   newSize * sizeof(seq_t));
        s2_curr   = (seq_t*)realloc(s2_curr,   newSize * sizeof(seq_t));

        /* Compute missing entries */
        for (idx_t i = max_comp + 1; i <= n; ++i)
        {
            /* powers of two: 2^i = 2 * 2^(i-1) */
            pow2[i] = pow2[i - 1] * 2;

            /* Stirling numbers of the first kind (unsigned) */
            if (i == 1)
            {
                s1_arr[1] = 1;      /* c(1,1) = 1 */
                s2_arr[1] = 0;      /* c(1,2) = 0 */
                s3_arr[1] = 0;      /* c(1,3) = 0 */
            }
            else
            {
                s1_arr[i] = (seq_t)(i - 1) * s1_arr[i - 1];
                s2_arr[i] = (seq_t)(i - 1) * s2_arr[i - 1] + s1_arr[i - 1];
                s3_arr[i] = (seq_t)(i - 1) * s3_arr[i - 1] + s2_arr[i - 1];
            }

            /* Stirling numbers of the second kind: build current row */
            s2_curr[0] = 0; /* S2(i,0) = 0 for i > 0 */
            for (idx_t k = 1; k <= i; ++k)
            {
                seq_t term1 = (seq_t)k * s2_prev[k];
                seq_t term2 = s2_prev[k - 1];
                s2_curr[k] = term1 + term2;
            }

            /* Compute a(i) = sum_{k=3..i} 2^(i-k) * S2(i,k) * S1(k,3) */
            seq_t sum = 0;
            for (idx_t k = 3; k <= i; ++k)
            {
                seq_t s1_signed = s3_arr[k];
                if (((k - 3) & 1) != 0)   /* (-1)^(k-3) */
                    s1_signed = -s1_signed;
                sum += pow2[i - k] * s2_curr[k] * s1_signed;
            }
            a_cache[i] = sum;

            /* Prepare for next iteration: swap rows */
            seq_t *tmp = s2_prev;
            s2_prev = s2_curr;
            s2_curr = tmp;
        }

        max_comp = n;
    }

    return a_cache[n];
}

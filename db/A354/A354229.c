/*
 * A354229  Expansion of e.g.f. 1/(1 - log(1 + x)^3).
 * https://oeis.org/A354229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 6 * Sum_{k=1..n} binomial(n,k) * Stirling1(k,3) * a(n-k)
 *            Initial values: a(0) = 1 (Stirling1(k,3) denotes the signed
 *            Stirling numbers of the first kind.)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 18, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* internal helper to grow an array of seq_t values */
static void *grow_array(void *ptr, size_t *cap, size_t needed, size_t elem_size)
{
    if (needed <= *cap) return ptr;
    size_t new_cap = *cap ? *cap * 2 : 64;
    while (new_cap <= needed) new_cap *= 2;
    ptr = realloc(ptr, new_cap * elem_size);
    *cap = new_cap;
    return ptr;
}

/* The main sequence function */
seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *a_vals = NULL;
    static idx_t a_cap = 0;
    static idx_t a_last = 0;        /* last computed index */

    /* array for Stirling numbers s(k,3) */
    static seq_t *s3_vals = NULL;
    static idx_t s3_cap = 0;
    static idx_t s3_last = 0;       /* last n for which s3[n] is computed */
    static seq_t prev_s[4] = { (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)0 }; /* row for n-1 */

    /* ---------- prepare Stirling numbers up to n ---------- */
    if (s3_last < n) {
        /* extend s3_vals array if needed */
        s3_vals = (seq_t *)grow_array(s3_vals, &s3_cap, n + 1, sizeof(seq_t));
        /* initialise first entries that are never used (k<3) */
        for (idx_t i = s3_last + 1; i <= n; ++i) {
            seq_t curr[4];
            curr[0] = (seq_t)0;
            curr[1] = prev_s[0] - (seq_t)(i - 1) * prev_s[1];
            curr[2] = prev_s[1] - (seq_t)(i - 1) * prev_s[2];
            curr[3] = prev_s[2] - (seq_t)(i - 1) * prev_s[3];
            s3_vals[i] = curr[3];
            /* advance prev_s to curr for next iteration */
            prev_s[0] = curr[0];
            prev_s[1] = curr[1];
            prev_s[2] = curr[2];
            prev_s[3] = curr[3];
        }
        s3_last = n;
    }

    /* ---------- prepare a[n] array up to n ---------- */
    if (a_vals == NULL) {
        a_vals = (seq_t *)malloc(64 * sizeof(seq_t));
        a_cap = 64;
        a_last = 0;
        /* set a(0)=1 */
        a_vals[0] = (seq_t)1;
    }
    if (a_last < n) {
        a_vals = (seq_t *)grow_array(a_vals, &a_cap, n + 1, sizeof(seq_t));
        for (idx_t i = a_last + 1; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            seq_t binom = (seq_t)1;
            for (idx_t k = 1; k <= i; ++k) {
                binom = binom * (seq_t)(i - k + 1) / (seq_t)k;
                if (s3_vals[k] == (seq_t)0) continue;   /* skip zero contributions */
                seq_t term = binom * s3_vals[k];
                term = term * a_vals[i - k];
                term = term * (seq_t)6;
                sum += term;
            }
            a_vals[i] = sum;
        }
        a_last = n;
    }

    return a_vals[n];
}

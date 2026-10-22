/*
 * A351423  Expansion of e.g.f. -log(1 - log(1 + log(1 + log(1 + log(1+x))))).
 * https://oeis.org/A351423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = T(n,5) * where * T(n,1) = (n-1)! * T(n,k) for k>1 =
 *            Sum_{j=1..n} Stirling1(n,j) * T(j,k-1) * Stirling1(n,j) =
 *            Stirling numbers of the first kind * (n, a(n)) are 1‑based; a(0)
 *            is unused
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 16, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: The function assumes seq_t and idx_t are supplied by the build. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* no value for indices below offset (n<1) */
    if (n == 0) return 0;

    /* Memoisation state */
    static idx_t maxN = 0;                      /* highest n computed so far   */
    static seq_t *T[6] = {NULL, NULL, NULL, NULL, NULL, NULL}; /* T[1]..T[5] */
    static seq_t *s_prev = NULL;                /* Stirling numbers for n-1   */
    static seq_t *s_curr = NULL;                /* Stirling numbers for n     */
    static seq_t fact = 1;                      /* (n-1)! for current n      */

    /* Grow storage if a larger n is requested */
    if (n > maxN) {
        size_t newlen = (size_t)n + 1;          /* indices 0 .. n inclusive   */

        /* Allocate or enlarge each T[k] array (k=1..5). */
        for (int k = 1; k <= 5; ++k) {
            T[k] = (seq_t *)realloc(T[k], newlen * sizeof(seq_t));
            if (!T[k]) abort();                 /* allocation failed           */
            /* initialise new entries to 0  */
            for (idx_t i = maxN + 1; i <= n; ++i)
                T[k][i] = 0;
        }

        /* Allocate or enlarge Stirling number arrays. */
        s_prev = (seq_t *)realloc(s_prev, newlen * sizeof(seq_t));
        s_curr = (seq_t *)realloc(s_curr, newlen * sizeof(seq_t));
        if (!s_prev || !s_curr) abort();        /* allocation failed           */
        for (idx_t i = maxN + 1; i <= n; ++i) {
            s_prev[i] = 0;
            s_curr[i] = 0;
        }

        /* Initialise s_prev[0] for the base case n=0. */
        if (maxN == 0) {
            s_prev[0] = 1;                       /* s(0,0)=1                    */
            fact       = 1;                       /* (0-1)! = 0! = 1             */
        }

        /* Compute terms for all new n (maxN+1 .. n). */
        for (idx_t m = maxN + 1; m <= n; ++m) {
            /* 1) Update factorial: T(m,1) = (m-1)! */
            if (m == 1)
                fact = 1;                          /* 0! = 1                      */
            else
                fact = fact * (m - 1);

            T[1][m] = fact;

            /* 2) Compute Stirling numbers of the first kind s(m,j) */
            s_curr[0] = 0;
            for (idx_t j = 1; j <= m; ++j) {
                seq_t a = s_prev[j - 1];           /* s(m-1,j-1) */
                seq_t b = (j <= m - 1) ? s_prev[j] : 0; /* s(m-1,j) */
                s_curr[j] = a - (seq_t)(m - 1) * b;
            }

            /* 3) Compute T(m,k) for k = 2 .. 5 */
            for (int k = 2; k <= 5; ++k) {
                seq_t sum = 0;
                for (idx_t j = 1; j <= m; ++j)
                    sum += s_curr[j] * T[k - 1][j];
                T[k][m] = sum;
            }

            /* 4) Prepare for next iteration: swap s_prev/s_curr */
            seq_t *tmp = s_prev;
            s_prev = s_curr;
            s_curr = tmp;
        }

        maxN = n;
    }

    /* Return the requested term. */
    return T[5][n];
}

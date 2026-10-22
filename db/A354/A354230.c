/*
 * A354230  Expansion of e.g.f. 1/(1 - log(1 + x)^5).
 * https://oeis.org/A354230
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 18, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A354230: a(n) = 120·Σ_{k=1..n} binomial(n,k)·Stirling1(k,5)·a(n-k),
 * with a(0)=1.  Stirling1(k,5) is the signed Stirling number of the first
 * kind; it vanishes for k<5, so the sum effectively starts at k=5. */

#include <stdlib.h>
#include <stdint.h>

/* The build provides the types seq_t (value type) and idx_t (uint64_t). */
seq_t a(idx_t n)
{
    /* ---------- first‑time initialisation ---------- */
    static seq_t *a_cache = NULL;   /* cache of a(0..i) */
    static size_t a_cap   = 0;       /* capacity of a_cache         */
    static idx_t  a_max   = 0;       /* highest index computed      */

    static seq_t *s_cache = NULL;   /* cache of Stirling1(k,5)     */
    static size_t s_cap   = 0;
    static idx_t  s_max   = 0;
    static seq_t c[6];              /* auxiliary for Stirling calc */

    /* initialise a_cache */
    if (!a_cache) {
        a_cap   = 1;
        a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
        a_cache[0] = (seq_t)1;      /* a(0)=1 */
        a_max = 0;
    }

    /* initialise s_cache and auxiliary array c */
    if (!s_cache) {
        s_cap   = 1;
        s_cache = (seq_t *)malloc(s_cap * sizeof(seq_t));
        s_cache[0] = (seq_t)0;      /* s(0,5)=0 */
        s_max = 0;
        c[0] = (seq_t)1;
        for (int j = 1; j < 6; ++j) c[j] = (seq_t)0;
    }

    /* ---- extend Stirling cache up to n ---- */
    if (s_max < n) {
        if (s_cap <= (size_t)n) {
            s_cache = (seq_t *)realloc(s_cache, (n + 1) * sizeof(seq_t));
            s_cap   = n + 1;
        }
        for (idx_t k = s_max + 1; k <= n; ++k) {
            /* compute unsigned Stirling numbers of the first kind
               using the recurrence c_j = c_{j-1} + (k-1)·c_j for j=5..1   */
            for (int j = 5; j >= 1; --j) {
                c[j] = c[j-1] + (seq_t)(k-1) * c[j];
            }
            c[0] = (seq_t)0;
            seq_t val = c[5];
            /* apply sign: (-1)^(k-5) */
            if ( (k-5) & 1u ) val = -val;
            s_cache[k] = val;
        }
        s_max = n;
    }

    /* ---- extend a_cache up to n ---- */
    if (a_max < n) {
        if (a_cap <= (size_t)n) {
            a_cache = (seq_t *)realloc(a_cache, (n + 1) * sizeof(seq_t));
            a_cap   = n + 1;
        }
        for (idx_t i = a_max + 1; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            seq_t C   = (seq_t)1;   /* binomial(i,0)=1 */
            for (idx_t k = 1; k <= i; ++k) {
                /* C = binomial(i,k) */
                __int128 tmp = (__int128)C * (i - k + 1);
                tmp /= k;
                C = (seq_t)tmp;

                if (k >= 5) {
                    sum += C * s_cache[k] * a_cache[i - k];
                }
            }
            a_cache[i] = sum * (seq_t)120;
        }
        a_max = n;
    }

    return a_cache[n];
}

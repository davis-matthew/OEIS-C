/*
 * A307355  E.g.f. A(x) satisfies: d/dx A(x) = 1 + A(x/(1 + x)).
 * https://oeis.org/A307355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   recurrence * a(1) = 1 * a(n+1) = Σ_{k=1..n} (-1)^(n-k) *
 *            binomial(n-1,k-1) * a(k) * n!/k!
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 18, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* local helper: multiplicative binomial(n,k) for seq_t */
static inline seq_t _binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return (seq_t)1;
    if (k > n - k) k = n - k;            /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * ((seq_t)(n - k + i));
        res = res / ((seq_t)i);
    }
    return res;
}

seq_t a(idx_t n)
{
    /* undefined for n == 0 – offset starts at 1 */
    if (n == 0) return (seq_t)0;

    /* static caches */
    static seq_t *a_cache   = NULL;   /* a(1)..a(max) */
    static idx_t  a_max     = 0;       /* largest index computed */
    static seq_t *fact_cache = NULL;  /* factorials 0..max_fact */
    static idx_t  fact_max   = 0;      /* largest factorial index stored */

    /* ------------- factorial cache up to n ------------- */
    if (fact_cache == NULL || fact_max < n) {
        idx_t new_max = n;
        if (fact_cache == NULL) {
            fact_cache = (seq_t *)realloc(NULL, (new_max+1) * sizeof(seq_t));
            fact_cache[0] = (seq_t)1;
            fact_max = 0;
        } else {
            fact_cache = (seq_t *)realloc(fact_cache, (new_max+1) * sizeof(seq_t));
        }
        if (fact_cache == NULL) return (seq_t)0;   /* out of memory */
        for (idx_t i = fact_max + 1; i <= new_max; ++i) {
            fact_cache[i] = fact_cache[i-1] * ((seq_t)i);
        }
        fact_max = new_max;
    }

    /* ------------- sequence cache up to n ------------- */
    if (a_cache == NULL || a_max < n) {
        idx_t new_max = n;
        if (a_cache == NULL) {
            a_cache = (seq_t *)realloc(NULL, (new_max+1) * sizeof(seq_t));
        } else {
            a_cache = (seq_t *)realloc(a_cache, (new_max+1) * sizeof(seq_t));
        }
        if (a_cache == NULL) return (seq_t)0;   /* out of memory */

        /* initialize known values */
        if (a_max == 0) {
            a_cache[1] = (seq_t)1;   /* a(1) = 1 */
            a_max = 1;
        }

        /* compute for m = a_max+1 .. n */
        for (idx_t m = a_max + 1; m <= new_max; ++m) {
            if (m == 1) {  /* safety, though handled above */
                a_cache[1] = (seq_t)1;
                continue;
            }
            seq_t sum = 0;
            idx_t N = m - 1;             /* factorial argument */
            seq_t fact_N = fact_cache[N]; /* (m-1)! */
            for (idx_t k = 1; k <= N; ++k) {
                /* a(k) */
                seq_t ak = a_cache[k];
                /* binomial(N-1, k-1) */
                seq_t c  = _binom(N-1, k-1);
                /* factorial ratio fact_N / fact_cache[k]  = N! / k! */
                seq_t ratio = fact_N / fact_cache[k];
                seq_t term = ak * c;
                term = term * ratio;
                /* sign */
                if (((N - k) & 1) != 0) sum -= term;
                else                  sum += term;
            }
            a_cache[m] = sum;
        }
        a_max = new_max;
    }

    return a_cache[n];
}

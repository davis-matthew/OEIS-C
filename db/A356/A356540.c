/*
 * A356540  Expansion of e.g.f. Product_{k>0} 1/(1 - (3 * x)^k)^(1/3^k).
 * https://oeis.org/A356540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 1
 *          a(n) = (n-1)! * Σ{k=1..n} A356539(k) * a(n-k)/(n-k)!   (n ≥ 1)
 *          where A356539(k) = Σ_{d|k} d * 3^{k-d}
 *
 * Implementation note:  The routine memoises previously computed values
 * in static arrays so that successive calls (in any order) are
 * answered in O(n) time from the first computed index to the maximum
 * queried index.  No table of fixed values is used.
 */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for memoisation */
    static idx_t      max_n   = 0;          /* largest n that has been computed */
    static seq_t *    A       = NULL;       /* A[k] = A356539(k)           */
    static seq_t *    fac     = NULL;       /* factorial values (idx_t!)   */
    static seq_t *    pow3    = NULL;       /* pow3[i] = 3^i                */
    static seq_t *    a_arr   = NULL;       /* a_arr[i] = a(i)              */

    /* initialisation ----------------------------------------------------- */
    if (A == NULL) {
        /* start with space for index 0 only */
        max_n = 0;
        A      = (seq_t *)malloc(sizeof(seq_t) * 1);
        fac    = (seq_t *)malloc(sizeof(seq_t) * 1);
        pow3   = (seq_t *)malloc(sizeof(seq_t) * 1);
        a_arr  = (seq_t *)malloc(sizeof(seq_t) * 1);
        if (!A || !fac || !pow3 || !a_arr) return (seq_t)0;  /* allocation failed */

        A[0]    = (seq_t)0;     /* defined so later loops never read it */
        fac[0]  = (seq_t)1;     /* 0! = 1                        */
        pow3[0] = (seq_t)1;     /* 3^0 = 1                        */
        a_arr[0] = (seq_t)1;    /* a(0) = 1                       */
    }

    /* extend precomputed range if necessary ------------------------------ */
    if (n > max_n) {
        idx_t new_n = n;

        /* reallocate arrays to accommodate indices up to new_n */
        A      = (seq_t *)realloc(A,      sizeof(seq_t) * (new_n + 1));
        fac    = (seq_t *)realloc(fac,    sizeof(seq_t) * (new_n + 1));
        pow3   = (seq_t *)realloc(pow3,   sizeof(seq_t) * (new_n + 1));
        a_arr  = (seq_t *)realloc(a_arr,  sizeof(seq_t) * (new_n + 1));
        if (!A || !fac || !pow3 || !a_arr) return (seq_t)0;   /* allocation failed */

        /* fill missing values for indices max_n+1 .. new_n ---------------- */
        /* factorials & powers of three */
        for (idx_t i = max_n + 1; i <= new_n; ++i) {
            pow3[i] = pow3[i - 1] * (seq_t)3;
            fac[i]  = fac[i - 1] * (seq_t)i;
            A[i]    = (seq_t)0;        /* placeholder, will compute below */
        }

        /* compute A[k] = Σ_{d|k} d * 3^{k-d} for new indices only */
        for (idx_t m = max_n + 1; m <= new_n; ++m) {
            seq_t sum = (seq_t)0;
            for (idx_t d = 1; d * d <= m; ++d) {
                if (m % d == 0) {
                    seq_t term1 = (seq_t)d * pow3[m - d];
                    sum += term1;
                    idx_t d2 = m / d;
                    if (d2 != d) {
                        seq_t term2 = (seq_t)d2 * pow3[m - d2];
                        sum += term2;
                    }
                }
            }
            A[m] = sum;
        }

        /* compute a_arr[i] for new indices */
        for (idx_t i = max_n + 1; i <= new_n; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= i; ++k) {
                seq_t term = A[k] * a_arr[i - k];
                /* ratio (i-1)! / (i-k)! = fac[i-1] / fac[i-k] */
                seq_t ratio = fac[i - 1] / fac[i - k];
                term *= ratio;
                sum += term;
            }
            a_arr[i] = sum;
        }

        max_n = new_n;
    }

    return a_arr[n];
}

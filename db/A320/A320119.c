/*
 * A320119  Expansion of Product_{k>0} 1/(Sum_{m>=0} x^(k*m^2)).
 * https://oeis.org/A320119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 26, i16 n <= 49, i32 n <= 52, i64 n <= 52, i128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A320119 – Expansion of Product_{k>0} 1/(Sum_{m>=0} x^(k*m^2))
   a(n) is the convolution inverse of A300446, i.e.
        sum_{k=0..n} a(k) * b(n-k) = delta_{n,0}
   where b(n) are the coefficients of
        B(x) = product_{k>0} (1 + x^k + x^{4k} + x^{9k} + …).
   The function memoises all intermediates so calls may arrive in any
   order.  */

#include <stdlib.h>
#include <string.h>

/* -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Memoised state ------------------------------------------ */
    static idx_t          max_n = 0;      /* largest n processed    */
    static seq_t *b      = NULL;        /* coefficients of B(x)   */
    static seq_t *a_arr  = NULL;        /* a(n) values            */
    static seq_t *dp     = NULL;        /* temporary array for B  */
    static seq_t *ndp    = NULL;        /* temporary array for B  */
    static size_t cap     = 0;          /* current allocation size*/

    /* Allocate or grow working arrays if needed ---------------- */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        free(b);
        free(a_arr);
        free(dp);
        free(ndp);
        b     = (seq_t*)calloc(new_cap, sizeof(seq_t));
        a_arr = (seq_t*)calloc(new_cap, sizeof(seq_t));
        dp    = (seq_t*)calloc(new_cap, sizeof(seq_t));
        ndp   = (seq_t*)calloc(new_cap, sizeof(seq_t));
        if (!b || !a_arr || !dp || !ndp) abort();
        cap = new_cap;

        /* ----------------------------------------------- */
        /* 1. Compute the coefficients b[0..n] of B(x)     */
        /* ----------------------------------------------- */
        dp[0] = 1;           /* start with product = 1                   */
        for (idx_t k = 1; k <= n; ++k) {
            /* ndp will hold the convolution of dp with
               P_k(x) = sum_{m>=0} x^{k*m^2}              */
            memset(ndp, 0, (n + 1) * sizeof(seq_t));

            /* m = 0 term (shift = 0) : copy dp as-is      */
            for (idx_t s = 0; s <= n; ++s) ndp[s] = dp[s];

            /* m >= 1 terms                                 */
            for (idx_t m = 1; ; ++m) {
                idx_t shift = k * m * m;   /* k*m^2 <= n */
                if (shift > n) break;
                for (idx_t s = shift; s <= n; ++s)
                    ndp[s] += dp[s - shift];
            }
            /* prepare for next k                            */
            seq_t *tmp = dp; dp = ndp; ndp = tmp;
        }
        memcpy(b, dp, (n + 1) * sizeof(seq_t));

        /* ----------------------------------------------- */
        /* 2. Compute the convolution inverse a[n]         */
        /* ----------------------------------------------- */
        a_arr[0] = 1;
        for (idx_t i = 1; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k)
                sum += a_arr[i - k] * b[k];
            a_arr[i] = -sum;
        }
        max_n = n;
    }

    return a_arr[n];
}

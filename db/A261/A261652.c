/*
 * A261652  Expansion of Product_{k>=0} ((1+x^(4*k+1))/(1-x^(4*k+1)))^3.
 * https://oeis.org/A261652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = coefficient of x^n in ∏_{k≥0} ((1 + x^(4k+1))/(1 –
 *            x^(4k+1)))^3. * * The code uses dynamic programming: for each
 *            factor * G_m(x) = ((1 + x^m)/(1 – x^m))^3, where m = 1 mod 4, *
 *            its coefficients are * coeff_t(t) = Σ_{i=0}^3 C(3,i) * C(t‑i+2,
 *            2) (t ≥ 0) * with the convention C(n,2)=0 for n...
 * Range:     u8 n <= 6, u16 n <= 22, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 38 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* coefficient of x^(t*m) in ((1+x^m)/(1-x^m))^3 */
static seq_t coeff_t(idx_t t)
{
    /*  C(n,2) = n*(n-1)/2 for n ≥ 2, 0 otherwise  */
    seq_t c0 = (seq_t)(t + 2) * (seq_t)(t + 1) / 2;   /* i = 0  */
    seq_t res = c0;
    if (t >= 1) {
        seq_t c1 = (seq_t)(t + 1) * (seq_t)(t) / 2;   /* i = 1  */
        res += 3 * c1;
    }
    if (t >= 2) {
        seq_t c2 = (seq_t)(t) * (seq_t)(t - 1) / 2;   /* i = 2  */
        res += 3 * c2;
    }
    if (t >= 3) {
        seq_t c3 = (seq_t)(t - 1) * (seq_t)(t - 2) / 2;/* i = 3  */
        res += c3;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* static arrays that grow with the largest n requested   */
    static seq_t *dp  = NULL;      /* current values            */
    static seq_t *dp2 = NULL;      /* temporary working array   */
    static size_t sz  = 0;          /* allocated length          */

    /* Recompute if a larger n is required.  The old dp contents
       are discarded; we start from scratch with a(0)=1.            */
    if (sz <= (size_t)n) {
        size_t new_sz = (size_t)n + 1;
        seq_t *new_dp  = mem_resize(seq_t, dp, new_sz);
        seq_t *new_dp2 = mem_resize(seq_t, dp2, new_sz);
        if (!new_dp || !new_dp2)      /* allocation failure – abort */
            return (seq_t)0;
        dp  = new_dp;
        dp2 = new_dp2;

        /* initialise array */
        for (size_t i = 0; i < new_sz; ++i)
            dp[i] = (seq_t)0;
        if (new_sz > 0)
            dp[0] = (seq_t)1;

        /* process all factors m = 1,5,9,... up to n */
        for (idx_t m = 1; m <= n; m += 4) {

            /* copy current dp into dp2 (t = 0 term) */
            for (size_t i = 0; i < new_sz; ++i)
                dp2[i] = dp[i];

            /* add t ≥ 1 terms */
            for (idx_t t = 1;; ++t) {
                idx_t step = t * m;
                if (step > (idx_t)(new_sz - 1))
                    break;
                seq_t ce = coeff_t(t);
                for (idx_t e = step; e < new_sz; ++e)
                    dp2[e] += dp[e - step] * ce;
            }

            /* prepare for next factor */
            seq_t *tmp = dp;
            dp = dp2;
            dp2 = tmp;
        }

        sz = new_sz;
    }

    return dp[n];
}

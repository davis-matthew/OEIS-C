/*
 * A271169  Number of ordered ways to write n as s^5 + t^5 + 2*u^5 + 3*v^5 +
 *          4*w^5 + 5*x^5 + 7*y^5 + 14*z^5, where s,t,u,v,w,x,y,z are
 *          nonnegative integers with s <= t.
 * https://oeis.org/A271169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A271169
   a(n) = number of ordered ways to write n as
          s^5 + t^5 + 2*u^5 + 3*v^5 + 4*w^5 + 5*x^5 + 7*y^5 + 14*z^5,
          with s,t,u,v,w,x,y,z ≥ 0 and s ≤ t.
   Computed by dynamic programming over the 8‑variable weighted sum. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t power5(idx_t base)
{
    seq_t r = 1;
    for (int i = 0; i < 5; ++i)
        r *= (seq_t)base;
    return r;
}

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------
       Determine the largest integer whose fifth power does not exceed n.
       ------------------------------------------------------------------ */
    idx_t max_base = 0;
    while (1) {
        seq_t next = power5(max_base + 1);
        if (next > n)
            break;
        ++max_base;
    }

    /* ------------------------------------------------------------------
       Pre‑compute all fifth powers up to max_base.
       ------------------------------------------------------------------ */
    size_t pow_cnt = (size_t)max_base + 1;
    seq_t *pow5 = (seq_t *)malloc(pow_cnt * sizeof(seq_t));
    if (!pow5) return 0;
    pow5[0] = 0;
    for (size_t i = 1; i < pow_cnt; ++i)
        pow5[i] = power5((idx_t)i);

    /* ------------------------------------------------------------------
       Setup DP array: dp[i] = number of ways to reach sum i using the first
       two variables s^5 + t^5 (with s ≤ t).  All other variables are added
       sequentially afterwards.
       ------------------------------------------------------------------ */
    size_t arr_size;
    if (n >= (idx_t)(SIZE_MAX - 1))
        return 0; /* cannot allocate array for extremely large n */
    arr_size = (size_t)n + 1;

    seq_t *dp = (seq_t *)calloc(arr_size, sizeof(seq_t));
    if (!dp) {
        free(pow5);
        return 0;
    }

    for (idx_t s = 0; s <= max_base; ++s) {
        seq_t ps = pow5[(size_t)s];
        for (idx_t t = s; t <= max_base; ++t) {
            seq_t pt = pow5[(size_t)t];
            seq_t sum = ps + pt;
            if (sum > n)
                break;
            dp[(size_t)sum] += (seq_t)1;
        }
    }

    /* ------------------------------------------------------------------
       Sequentially add each of the remaining six variables.
       ------------------------------------------------------------------ */
    const idx_t coeffs[6] = {2, 3, 4, 5, 7, 14};

    for (int ci = 0; ci < 6; ++ci) {
        idx_t coeff = coeffs[ci];

        seq_t *next = (seq_t *)calloc(arr_size, sizeof(seq_t));
        if (!next) {
            free(dp);
            free(pow5);
            return 0;
        }

        for (size_t sum = 0; sum < arr_size; ++sum) {
            seq_t ways = dp[sum];
            if (ways == 0)
                continue;
            for (idx_t k = 0; k <= max_base; ++k) {
                seq_t val = (seq_t)coeff * pow5[(size_t)k];
                seq_t new_sum = (seq_t)sum + val;
                if (new_sum > n)
                    break;
                next[(size_t)new_sum] += ways;
            }
        }

        free(dp);
        dp = next;
    }

    seq_t result = dp[(size_t)n];
    free(dp);
    free(pow5);
    return result;
}

/*
 * A244787  Number of length n 0..7 arrays with each partial sum starting from
 *          the beginning no more than one standard deviation from its mean.
 * https://oeis.org/A244787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A244787
    Number of length‑n arrays with digits 0..7 such that every partial
    sum S_k satisfies |S_k – 3.5k| ≤ √(5.25k).  The algorithm
    computes the admissible range for each prefix length k and
    uses dynamic programming over that range. */

#include <stdlib.h>
#include <stdint.h>

/* Compute ⌊√x⌋ for a 64‑bit unsigned integer.  Uses 128‑bit
   arithmetic to avoid overflow. */
static idx_t floor_isqrt_u64(idx_t x)
{
    if (x < 2)
        return x;
    idx_t lo = 0;
    idx_t hi = x;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq == x)
            return mid;
        if (sq < x) {
            lo = mid + 1;
        } else {
            if (mid == 0)
                break;
            hi = mid - 1;
        }
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n < 1u)
        return (seq_t)0u;

    /* Pre‑compute admissible ranges for each prefix length. */
    idx_t *lo = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    idx_t *hi = (idx_t *)malloc((n + 1) * sizeof(idx_t));
    if (!lo || !hi) {
        free(lo);
        free(hi);
        return (seq_t)0u;
    }

    idx_t maxwidth = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t t = floor_isqrt_u64(21 * k);           /* floor(√(21k)) */
        idx_t num_low = 7 * k - t;
        idx_t low_k = num_low / 2;                   /* floor((7k – t)/2) */
        if (num_low & 1u)                            /* ceil if odd */
            ++low_k;
        idx_t high_k = (7 * k + t) / 2;              /* floor((7k + t)/2) */

        lo[k] = low_k;
        hi[k] = high_k;
        idx_t width = high_k - low_k + 1;
        if (width > maxwidth)
            maxwidth = width;
    }

    /* Allocate two DP rows of size maxwidth. */
    seq_t *dp1 = (seq_t *)calloc((size_t)maxwidth, sizeof(seq_t));
    seq_t *dp2 = (seq_t *)calloc((size_t)maxwidth, sizeof(seq_t));
    if (!dp1 || !dp2) {
        free(lo);
        free(hi);
        free(dp1);
        free(dp2);
        return (seq_t)0u;
    }

    /* Initial state: partial sum 0, one sequence. */
    dp1[0] = (seq_t)1u;
    idx_t prev_low = 0;
    idx_t prev_size = 1;

    /* Iterate over positions 1..n. */
    for (idx_t step = 1; step <= n; ++step) {
        idx_t cur_low = lo[step];
        idx_t cur_high = hi[step];
        idx_t cur_width = cur_high - cur_low + 1;

        /* Clear next DP row. */
        for (idx_t i = 0; i < cur_width; ++i)
            dp2[i] = (seq_t)0u;

        /* Transition from previous DP row. */
        for (idx_t i = 0; i < prev_size; ++i) {
            seq_t v = dp1[i];
            if (!v)
                continue;
            idx_t sum_prev = prev_low + i;
            for (int d = 0; d <= 7; ++d) {
                idx_t sum_new = sum_prev + (idx_t)d;
                if (sum_new < cur_low || sum_new > cur_high)
                    continue;
                idx_t idx_new = sum_new - cur_low;
                dp2[idx_new] += v;
            }
        }

        /* Prepare for next iteration. */
        seq_t *tmp = dp1;
        dp1 = dp2;
        dp2 = tmp;
        prev_low = cur_low;
        prev_size = cur_width;
    }

    /* Sum final DP row to get the answer. */
    seq_t total = (seq_t)0u;
    for (idx_t i = 0; i < prev_size; ++i)
        total += dp1[i];

    /* Clean up. */
    free(lo);
    free(hi);
    free(dp1);
    free(dp2);

    return total;
}

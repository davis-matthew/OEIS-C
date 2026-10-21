/*
 * A244831  Number of length n 0..7 arrays with each partial sum starting from
 *          the beginning no more than two standard deviations from its mean.
 * https://oeis.org/A244831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length‑n arrays of digits 0..7 such that for
 *            every k = 1..n the partial sum S_k = a1 + a2 + … + a_k satisfies
 *            |S_k – k·3.5| ≤ 2·sqrt(k·5.25)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Integer square root for 0 ≤ n < 2^64.
   Returns ⌊√n⌋.  Implemented with a binary search to avoid
   floating‑point operations. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (mid > 0 && mid > n / mid) {
            high = mid - 1;
        } else {
            ans = mid;
            low = mid + 1;
        }
    }
    return ans;
}

/* The required sequence function. */
seq_t a(idx_t n)
{
    /* a(0) is undefined for this sequence; return 0. */
    if (n == 0) return 0;

    /* Arrays for dynamic programming.  At each step k we keep the
       counts of all partial sums S_k that are allowed for that k. */
    seq_t *dp_prev = NULL;
    seq_t *dp_curr = NULL;
    idx_t len_prev = 0;
    idx_t len_curr = 0;
    idx_t L_prev = 0, U_prev = 0;
    idx_t L_curr = 0, U_curr = 0;

    /* ----- k = 1 ----- */
    idx_t k = 1;
    idx_t seven_k = 7 * k;
    idx_t t = isqrt_u64(84 * k);                /* ⌊√(84k)⌋ = 2·√(21k) */
    /* Compute lower bound L = ceil((7k – t)/2), clamped to ≥0. */
    if (seven_k > t)
        L_curr = (seven_k - t + 1) / 2;
    else
        L_curr = 0;
    /* Upper bound U = floor((7k + t)/2). */
    U_curr = (seven_k + t) / 2;
    len_curr = U_curr - L_curr + 1;
    dp_curr = (seq_t *)calloc((size_t)len_curr, sizeof(seq_t));
    if (!dp_curr) return 0;                 /* allocation failure */

    /* Initialize for all possible first digits 0..7. */
    for (idx_t v = 0; v <= 7; ++v) {
        idx_t sum = v;
        if (sum >= L_curr && sum <= U_curr) {
            dp_curr[sum - L_curr] += 1;
        }
    }
    /* Prepare for the next iteration. */
    dp_prev = dp_curr;
    L_prev = L_curr;
    U_prev = U_curr;
    len_prev = len_curr;

    /* ----- k = 2 … n ----- */
    for (k = 2; k <= n; ++k) {
        seven_k = 7 * k;
        t = isqrt_u64(84 * k);
        if (seven_k > t)
            L_curr = (seven_k - t + 1) / 2;
        else
            L_curr = 0;
        U_curr = (seven_k + t) / 2;
        len_curr = U_curr - L_curr + 1;
        dp_curr = (seq_t *)calloc((size_t)len_curr, sizeof(seq_t));
        if (!dp_curr) {
            free(dp_prev);
            return 0;                /* allocation failure */
        }

        /* Transition from previous step. */
        for (idx_t i = 0; i < len_prev; ++i) {
            seq_t cnt = dp_prev[i];
            if (!cnt) continue;
            idx_t s_prev = L_prev + i;
            for (idx_t v = 0; v <= 7; ++v) {
                idx_t s_new = s_prev + v;
                if (s_new < L_curr || s_new > U_curr) continue;
                dp_curr[s_new - L_curr] += cnt;
            }
        }

        free(dp_prev);
        dp_prev = dp_curr;
        L_prev = L_curr;
        U_prev = U_curr;
        len_prev = len_curr;
    }

    /* Sum all remaining counts to get the answer. */
    seq_t result = 0;
    for (idx_t i = 0; i < len_prev; ++i) {
        result += dp_prev[i];
    }

    free(dp_prev);
    return result;
}

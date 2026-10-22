/*
 * A318771  Expansion of Sum_{k>=0} x^(k^2) / Product_{j=1..k} (1 - x^j)^j.
 * https://oeis.org/A318771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k>=0} coefficient of x^{n-k^2} in Prod_{j=1..k} (1 -
 *            x^j)^(-j) This implementation builds the product incrementally
 *            and uses a simple integer square‑root routine
 * Range:     u8 n <= 21, u16 n <= 44, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ---- simple integer sqrt for idx_t (uint64_t) ---- */
static idx_t isqrt_u64(idx_t v)
{
    idx_t lo = 0, hi = v, mid, res = 0;
    while (lo <= hi) {
        mid = lo + (hi - lo) / 2;
        if (mid * mid <= v) {
            res = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return res;
}

/* m(n) = a(n) ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* allocate or resize workspace if needed (static for reuse) */
    static seq_t *dp = NULL;
    static size_t cap = 0;
    size_t need = (size_t)n + 1;

    if (dp == NULL || cap < need) {
        seq_t *tmp = (seq_t *)malloc(need * sizeof(seq_t));
        if (dp != NULL) free(dp);
        dp   = tmp;
        cap  = need;
    }

    /* initialise coefficient array for current product. */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

    seq_t result = 0;

    idx_t kmax = isqrt_u64(n);

    for (idx_t k = 0; k <= kmax; ++k) {
        idx_t shift = k * k;
        if (n >= shift) {
            result = result + dp[n - shift];
        }

        if (k == kmax) break;  /* no further multiplication needed */

        /* multiply by (1 - x^{k+1})^{-(k+1)}  */
        idx_t j = k + 1;
        for (idx_t copy = 1; copy <= j; ++copy) {
            for (idx_t t = j; t <= n; ++t) {
                dp[t] = dp[t] + dp[t - j];
            }
        }
    }

    return result;
}

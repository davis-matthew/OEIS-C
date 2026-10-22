/*
 * A394291  Number of partitions of n such that the multiplicity m of the
 *          least part s is a part of the partition and m > s.
 * https://oeis.org/A394291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n such that the multiplicity m of
 *            the least part s is a part of the partition and m > s.
 *            Equivalent to a(n) = Sum_{s>=1} Sum_{m>=s+1} p_{>=s+1}( n - s*m
 *            - m ), where p_{>=k}(x) is the number of partitions of x into
 *            parts all at least k. The algorithm builds the part...
 * Range:     u8 n <= 22, u16 n <= 50, u32 n <= 137, u64 n <= 430,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static idx_t floor_isqrt_u64(idx_t x)
{
    idx_t low = 0, high = x, mid, res = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            res = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 4) return 0;                     /* trivial case */

    const idx_t N = n;
    /* dp[i] will contain the number of partitions of i with all parts >= current_min. */
    seq_t *dp = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    if (!dp) return 0;                       /* allocation failure */

    dp[0] = 1;
    /* initial unrestricted partitions p_{>=1} */
    for (idx_t part = 1; part <= N; ++part) {
        for (idx_t i = part; i <= N; ++i) {
            dp[i] = dp[i] + dp[i - part];
        }
    }

    seq_t result = 0;
    idx_t sqrt_n = floor_isqrt_u64(N);
    idx_t max_s = (sqrt_n > 0) ? sqrt_n - 1 : 0;   /* s satisfies (s+1)^2 <= N */

    idx_t current_min = 1;                        /* currently dp holds p_{>=1} */
    for (idx_t s = 1; s <= max_s; ++s) {
        /* transition to partitions with minimum part = current_min + 1 = s+1 */
        for (idx_t i = N; i >= current_min; --i) {
            dp[i] = dp[i] - dp[i - current_min];
        }
        current_min++; /* now dp holds p_{>=s+1} */

        idx_t min_m = current_min;                /* minimal multiplicity m = s+1 */
        idx_t max_m = N / min_m;                  /* m*(s+1) <= N */

        for (idx_t m = min_m; m <= max_m; ++m) {
            idx_t rem = N - m * min_m;            /* n - s*m - m = n - m*(s+1) */
            result = result + dp[rem];
        }
    }

    free(dp);
    return result;
}

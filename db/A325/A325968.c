/*
 * A325968  a(n) is the sum k of such a subset of divisors of n with the
 *          largest sum and for which n-k and n-(sigma(n)-k) are relatively
 *          prime.
 * https://oeis.org/A325968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum sum k of a subset of divisors of n such that
 *            gcd(n-k, n-(σ(n)-k)) = 1. The function enumerates all subset
 *            sums of the divisors of n (the number of divisors of n ≤ 10000
 *            is at most 256) and scans them from largest to smallest to find
 *            the first one satisfying the coprimality condition
 * Range:     u8 n <= 107, u16 n <= 9972, u32 n <= 9972, u64 n <= 9972,
 *            u128 n <= 9972, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/gcd.h"   /* seq_t gcd(seq_t, seq_t) */

static const size_t MAX_DIVISORS = 256; /* safe upper bound for n ≤ 10000 */

/* Return the A325968 value for index n.  The function works for any
   sequence type seq_t that supports + - * / % and comparison */
seq_t a(idx_t n)
{
    seq_t sigma = 0;                 /* sum of all divisors of n  */
    uint64_t divisors[MAX_DIVISORS];
    size_t dcnt = 0;                 /* number of divisors found   */

    /* Find all divisors of n and accumulate sigma */
    for (idx_t i = 1; i <= n; ++i) {
        if (n % i == 0) {
            divisors[dcnt++] = (uint64_t)i;
            sigma += (seq_t)i;
        }
    }

    /* Allocate DP array: bit mask of achievable sums (0..sigma) */
    size_t dp_size = (size_t)sigma + 1;              /* number of sums */
    unsigned char *dp = (unsigned char *)calloc(dp_size, 1);
    if (!dp) return 0;      /* safe‑guard, should not happen for small n */

    dp[0] = 1; /* sum 0 always achievable (empty subset) */

    /* Subset‑sum DP: for each divisor update dp from high to low */
    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t v = divisors[i];
        /* iterate sums downward */
        for (long long s = (long long)(dp_size - 1); s >= (long long)v; --s) {
            if (dp[s - v]) dp[s] = 1;
        }
    }

    /* Compute constant 2*n - sigma once as signed 64‑bit */
    int64_t two_n_minus_sigma = 2 * (int64_t)n - (int64_t)sigma;

    /* Scan sums from largest to smallest to find the first valid k */
    seq_t answer = 0; /* default for safety (should always be set) */
    for (long long k_ll = (long long)sigma; k_ll >= 0; --k_ll) {
        if (!dp[(size_t)k_ll]) continue;  /* sum k not achievable */

        seq_t k = (seq_t)k_ll;
        int64_t A = (int64_t)n - k_ll;            /* n - k */
        int64_t B = two_n_minus_sigma - A;        /* 2n - σ(n) - A */

        seq_t absA = (A < 0) ? (seq_t)(-A) : (seq_t)A;
        seq_t absB = (B < 0) ? (seq_t)(-B) : (seq_t)B;

        if (gcd(absA, absB) == 1) {
            answer = k;
            break;
        }
    }

    free(dp);
    return answer;
}

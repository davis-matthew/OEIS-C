/*
 * A247654  Write 4n as a product of numbers of the form 4k+2 (A016825) so as
 *          to minimize the sum of the factors; a(n) is this sum.
 * https://oeis.org/A247654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * S(m,n) where m = v₂(n) + 2 and S(m,n) is the minimal
 *            sum of m odd factors whose product equals the odd part of n. The
 *            implementation uses memoized recursion on the odd part and the
 *            factor count
 * Range:     u8 n <= 126, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ctz_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/ctz_u64.h"

/* Maximum odd part we cache for speed */
#define MAX_ODD 20000
/* Maximum number of odd factors we cache */
#define MAX_M 20

/* DP table: dp[odd][m] stores minimal sum for odd <= MAX_ODD. */
static seq_t dp[MAX_ODD + 1][MAX_M + 1];

/* Recursive computation of minimal sum of m odd factors whose product is odd. */
static seq_t min_sum_odd(seq_t odd, idx_t m)
{
    if (m == 1) {
        return odd;
    }
    if (odd == 1) {
        return (seq_t)m;
    }

    idx_t odd_i = (idx_t)odd;
    if (odd_i <= MAX_ODD) {
        seq_t cached = dp[odd_i][m];
        if (cached != 0) {
            return cached;
        }
    }

    /* Baseline: one factor equal to odd, the rest 1. */
    seq_t best = odd + (seq_t)(m - 1);

    idx_t limit = odd_i;
    for (idx_t d = 3; d <= limit / d; d += 2) {
        if (odd_i % d == 0) {
            seq_t d_seq = (seq_t)d;
            seq_t e_seq = odd / d_seq;
            for (idx_t k = 1; k < m; ++k) {
                seq_t s1 = min_sum_odd(d_seq, k);
                seq_t s2 = min_sum_odd(e_seq, m - k);
                seq_t sum = s1 + s2;
                if (sum < best) {
                    best = sum;
                }
            }
        }
    }

    if (odd_i <= MAX_ODD) {
        dp[odd_i][m] = best;
    }
    return best;
}

/* Public function: a(n) */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    /* 4n = 2^m * odd_part, with m = v₂(n) + 2 */
    idx_t v2 = ctz_u64((uint64_t)n);
    idx_t m  = v2 + 2;
    seq_t odd_part = (seq_t)(n >> v2);

    seq_t odd_sum = min_sum_odd(odd_part, m);
    return odd_sum * (seq_t)2;
}

/*
 * A297053  Sum of the larger parts of the partitions of n into two parts such
 *          that the smaller part does not divide the larger.
 * https://oeis.org/A297053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} (n-i) * (1 - (floor(n/i) -
 *            floor((n-1)/i)))
 * Range:     u8 n <= 28, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt_u64(idx_t n) {
    /* integer square root: floor(sqrt(n)) */
    idx_t lo = 0, hi = ((idx_t)1 << 32);
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if ((__int128)mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n) {
    if (n <= 1) return 0; /* early return for small n */

    idx_t m = n / 2;                      /* floor(n/2) */
    seq_t nseq = (seq_t)n;
    seq_t mseq = (seq_t)m;

    /* sum of (n-i) for i=1..m */
    seq_t sum_all = mseq * nseq - (mseq * (m + 1) / 2);

    /* count divisors d of n with d <= m, and sum them */
    seq_t D = 0;   /* count of such divisors */
    seq_t S = 0;   /* sum of such divisors */

    idx_t sqrt_n = isqrt_u64(n);
    for (idx_t i = 1; i <= sqrt_n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            if (d1 <= m) {
                ++D;
                S += (seq_t)d1;
            }
            if (d2 != d1 && d2 <= m) {
                ++D;
                S += (seq_t)d2;
            }
        }
    }

    seq_t subtract_term = nseq * D - S;
    return sum_all - subtract_term;
}

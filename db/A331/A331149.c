/*
 * A331149  Triangle read by rows: T(n,k) (n>=k>=1) =
 *          floor((n/k)*ceiling(n/k)).
 * https://oeis.org/A331149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = floor((r/k) * ceil(r/k)) where * r = smallest integer
 *            with r*(r+1)/2 >= n, * k = n - (r-1)*r/2
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;                  /* guard for out-of-range */
    /* Find r by binary search. r <= n for all n >= 1. */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t val = (__uint128_t)mid * (mid + 1) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Compute k = n - T_{r-1}. */
    __uint128_t prev_prod = (__uint128_t)(r - 1) * r;
    idx_t prev_tri = (idx_t)(prev_prod / 2);
    idx_t k = n - prev_tri;

    /* Compute floor((r/k) * ceil(r/k)). */
    seq_t sr = (seq_t)r;
    seq_t sk = (seq_t)k;
    seq_t q = sr / sk;
    seq_t rmod = sr % sk;
    seq_t ceil = (rmod == 0) ? q : q + 1;
    return (sr * ceil) / sk;
}

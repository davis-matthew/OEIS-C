/*
 * A284444  a(n) = Sum_{d|n, d == 3 (mod 7)} d.
 * https://oeis.org/A284444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d divides n, d ≡ 3 (mod 7)} d
 * Range:     u8 n <= 247, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root for uint64_t values.
   Returns ⌊√n⌋ and never overflows for n up to 2⁶⁴−1. */
static idx_t isqrt_u64(idx_t n) {
    if (n < 2) return n;
    /* The maximum possible square root of 2⁶⁴−1 is 0xFFFFFFFF. */
    idx_t low = 1;
    idx_t high = (n > 0xFFFFFFFFULL) ? 0xFFFFFFFFULL : n;
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    if (n == 0) return sum;  /* offset starts at 1; safe guard */
    idx_t r = isqrt_u64(n);
    for (idx_t i = 1; i <= r; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;
            if (d1 % 7 == 3) {
                sum += (seq_t)d1;
            }
            if (d2 != d1 && (d2 % 7 == 3)) {
                sum += (seq_t)d2;
            }
        }
    }
    return sum;
}

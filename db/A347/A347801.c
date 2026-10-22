/*
 * A347801  Expansion of ( Sum_{k>=0} k^2 * q^(k^2) )^2.
 * https://oeis.org/A347801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i^2 + j^2 = n, i>0, j>0} i^2 · j^2
 * Range:     u8 n <= 24, u16 n <= 324, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root for uint64_t via binary search.
 * Returns ⌊sqrt(x)⌋. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = x, mid, res = 0;
    while (low <= high) {
        mid = (low + high) / 2;
        idx_t sq = mid * mid;
        if (sq <= x) {
            res = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;                /* no solutions for n = 0 or 1 */
    seq_t sum = 0;

    /* i must satisfy 1 ≤ i ≤ √(n‑1) so that j² = n‑i² ≥ 1 */
    idx_t max_i = isqrt_u64(n - 1);
    for (idx_t i = 1; i <= max_i; ++i) {
        idx_t rem = n - i * i;         /* n – i²   */
        /* rem > 0 by construction; find j with j² = rem */
        idx_t j = isqrt_u64(rem);
        if (j * j != rem) continue;    /* not a perfect square, skip */

        /* compute i²·j² efficiently in seq_t */
        seq_t i2 = (seq_t)i * (seq_t)i;
        seq_t j2 = (seq_t)j * (seq_t)j;
        sum += i2 * j2;
    }
    return sum;
}

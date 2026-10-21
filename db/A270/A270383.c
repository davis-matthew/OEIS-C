/*
 * A270383  Number of ordered pairs (i,j) with i >= j, |i|, |j| <= n, and |i *
 *          j| <= n.
 * https://oeis.org/A270383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(∑_{k=1..n} τ(k)) + floor(sqrt(n)) + 2*n + 1
 * Range:     u8 n <= 27, u16 n <= 3512, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root for unsigned 64‑bit values */
static inline idx_t isqrt_u64(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t low = 1, high = n, ans = 1;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid <= n / mid) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* sum_{k=1..n} τ(k) = ∑_{d=1..n} floor(n/d) */
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ) {
        idx_t q    = n / i;          /* floor(n / i) */
        idx_t next = n / q + 1;       /* first index where floor(n / i) changes */
        sum += (seq_t)(next - i) * (seq_t)q;
        i = next;
    }

    idx_t sqrt_n = isqrt_u64(n);
    seq_t result = 2 * sum + (seq_t)sqrt_n + (seq_t)2 * (seq_t)n + 1;
    return result;
}

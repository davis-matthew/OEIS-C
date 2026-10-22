/*
 * A396205  a(n) = number of integers k <= n with g(k)^3 < k, where g(k) is
 *          the largest divisor d of k satisfying d <= sqrt(k) (A033676).
 * https://oeis.org/A396205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{ k <= n | g(k)^3 < k } where g(k) = largest divisor d
 *            of k with d <= sqrt(k)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t floor_sqrt(idx_t n)
{
    if (n < 2) return n;
    idx_t low = 1, high = n, mid;
    while (low <= high) {
        mid = low + (high - low) / 2;
        if (mid <= n / mid)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t count = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t sqrt_k = floor_sqrt(k);
        idx_t g = 1;
        for (idx_t d = sqrt_k; d >= 1; --d) {
            if (k % d == 0) {
                g = d;
                break;
            }
        }
        seq_t g3 = (seq_t)g * (seq_t)g * (seq_t)g;
        if (g3 < (seq_t)k) ++count;
    }
    return count;
}

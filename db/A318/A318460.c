/*
 * A318460  a(n) = Sum_{d|n, d < n/d} (d XOR n/d), where XOR is bitwise-xor
 *          (A003987).
 * https://oeis.org/A318460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{d|n, d < n/d} (d XOR n/d)
 * Range:     u8 n <= 107, u16 n <= 9794, u32 n <= 9794, u64 n <= 9794,
 *            u128 n <= 9794, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(n)) for 0 <= n <= UINT64_MAX */
static idx_t floor_sqrt_u64(idx_t n)
{
    if (n == 0) return 0;
    idx_t low = 1;
    idx_t high = n;
    idx_t ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        /* mid*mid may overflow 64-bit, compare via division */
        if (mid > n / mid) {
            high = mid - 1;
        } else {
            ans = mid;
            low = mid + 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    idx_t sqrt_n = floor_sqrt_u64(n);
    seq_t sum = 0;
    for (idx_t d = 1; d <= sqrt_n; ++d) {
        if (n % d == 0) {
            idx_t q = n / d;
            if (d < q) {
                seq_t x = (seq_t)d ^ (seq_t)q;
                sum += x;
            }
        }
    }
    return sum;
}

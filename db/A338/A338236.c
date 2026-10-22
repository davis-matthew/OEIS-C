/*
 * A338236  Number of numbers less than or equal to sqrt(n) whose square does
 *          not divide n.
 * https://oeis.org/A338236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(sqrt(n)) – #{k | 1 ≤ k ≤ floor(sqrt(n)) and k²
 *            divides n}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for 64‑bit values.  Returns ⌊√n⌋. */
static idx_t isqrt_u64(idx_t n)
{
    if (n < 2) return n;
    idx_t low  = 0, high = n, ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {          /* avoid division by zero */
            ans  = 0;
            low  = 1;
            continue;
        }
        if (mid <= n / mid) {    /* mid² ≤ n without overflow */
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
    idx_t r = isqrt_u64(n);       /* floor(sqrt(n)) */
    seq_t count = 0;              /* number of k such that k² | n */

    for (idx_t k = 1; k <= r; ++k) {
        uint64_t k2 = (uint64_t)k * (uint64_t)k;
        if (n % k2 == 0) {
            ++count;
        }
    }
    return (seq_t)r - count;      /* k with k² NOT dividing n */
}

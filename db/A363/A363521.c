/*
 * A363521  Product of the divisors d of n such that sqrt(n) < d < n.
 * https://oeis.org/A363521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{d|n, sqrt(n) < d < n} d
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(sqrt(n)) for n up to 2^64-1 using binary search */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    if (n < 2) return n;            /* sqrt(0)=0, sqrt(1)=1 */
    idx_t low = 0;
    idx_t high = n >> 1;            /* sqrt(n) <= n/2 for n >= 4 */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid <= n / mid)        /* mid*mid <= n  (avoid overflow) */
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Return a(n): product of all divisors d of n such that sqrt(n) < d < n */
seq_t a(idx_t n)
{
    if (n <= 1)            /* a(1) = 1 */
        return (seq_t)1;

    seq_t result = 1;
    idx_t sqrt_n = floor_sqrt_u64(n);

    /* Skip d = 1 because its partner is n, which must be excluded. */
    for (idx_t d = 2; d <= sqrt_n; ++d) {
        if ((n % d) == 0) {
            if (d * d == n)    /* skip the square root if n is a perfect square */
                continue;
            /* partner = n / d > sqrt(n) and < n */
            seq_t partner = (seq_t)(n / d);
            result *= partner;
        }
    }

    return result;
}

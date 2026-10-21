/*
 * A262202  Number of divisors d | n such that d^2 < n and d^2 does not divide
 *          n.
 * https://oeis.org/A262202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n such that d² < n and d² ∤ n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square‑root (floor), safe for 64‑bit inputs. */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = n;
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        if (mid == 0 || mid > n / mid)
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

/* Count the required divisors of n. */
seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    if (N <= 1)
        return 0;

    /* All d with d² < N satisfy d ≤ √(N−1). */
    uint64_t limit = isqrt_u64(N - 1U);

    seq_t count = 0;
    for (uint64_t d = 1; d <= limit; ++d) {
        if (N % d == 0) {
            uint64_t d2 = d * d;           /* d² ≤ N−1, fits in 64 bits */
            if (N % d2 != 0)               /* d² does not divide N */
                ++count;
        }
    }
    return count;
}

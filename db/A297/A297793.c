/*
 * A297793  a(n) = Sum_{d|n} min(d, n/d)^3.
 * https://oeis.org/A297793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} min(d, n/d)^3
 * Range:     u8 n <= 29, u16 n <= 671, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for 64‑bit unsigned values.
   Returns the largest integer s such that s*s <= n. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n, mid, best = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        if (mid == 0) {
            best = 0;
            low = 1;
            continue;
        }
        if (mid <= n / mid) {          /* mid*mid <= n */
            best = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;       /* Prevent underflow */
            high = mid - 1;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    idx_t sq = isqrt_u64(n);          /* floor(sqrt(n)) */
    seq_t sum = 0;
    for (idx_t d = 1; d <= sq; ++d) {
        if (n % d == 0) {
            seq_t cube = (seq_t)d * (seq_t)d * (seq_t)d;
            sum += cube;
        }
    }
    seq_t result = sum * 2;
    if (sq * sq == n) {               /* perfect square: subtract sqrt^3 once */
        seq_t cube = (seq_t)sq * (seq_t)sq * (seq_t)sq;
        result -= cube;
    }
    return result;
}

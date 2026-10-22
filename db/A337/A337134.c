/*
 * A337134  a(n) = Sum_{k=1..n} floor(sqrt(2k-1)).
 * https://oeis.org/A337134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} floor(sqrt(2k-1))
 * Range:     u8 n <= 43, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(sqrt(n)) for a 128‑bit value n using only integer arithmetic.
   The result fits in seq_t, which is at least 8 bits. */
static inline seq_t floor_sqrt_128(unsigned __int128 n)
{
    seq_t low = 0;
    seq_t high = (seq_t)-1U;          /* maximum value representable by seq_t */

    while (low <= high) {
        seq_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq == n) return mid;
        if (sq < n) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;  /* high is floor(sqrt(n)) when loop exits */
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        unsigned __int128 odd = ((unsigned __int128)k << 1) - 1;  /* 2k-1 */
        seq_t val = floor_sqrt_128(odd);
        sum += val;
    }
    return sum;
}

/*
 * A394687  Number of ordered pairs of positive integers (a,b) with a < b such
 *          that b^2 - a^2 <= n.
 * https://oeis.org/A394687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..floor((n-1)/2)} ( floor( sqrt(k^2 + n) ) - k )
 *            where sqrt() is the integer square‑root (floor)
 * Range:     u8 n <= 14, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t limit = (n - 1) / 2;          /* last k to consider */
    for (idx_t k = 1; k <= limit; ++k) {
        seq_t kk  = (seq_t)k * (seq_t)k;   /* k^2 */
        seq_t val = kk + (seq_t)n;         /* k^2 + n */
        seq_t s   = floor_sqrt(val);       /* floor sqrt(k^2 + n) */
        result += s - (seq_t)k;            /* add contribution for this k */
    }
    return result;
}

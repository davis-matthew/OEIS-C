/*
 * A337976  Number of partitions of n into two distinct parts (s,t), such that
 *          s | t, (t-s) | n, and where n/(t-s) <= s < t.
 * https://oeis.org/A337976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = |{ s ∈ ℕ : 1 ≤ s < n/2,
 *   n % s == 0,   d = n - 2 s,
 *   n % d == 0,   n/d ≤ s }|
 * where d = n - 2 s (> 0).
 * This counts partitions of n into two distinct parts (s, t)
 * with s | t, (t - s) | n, and n/(t - s) ≤ s < t.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    /* iterate over possible s < t, i.e. 1 ≤ s < n/2 */
    for (idx_t s = 1; s < n / 2; ++s) {
        /* s must divide n (since s | t ⇔ s | n) */
        if (n % s != 0)
            continue;
        idx_t d = n - 2 * s;   /* d = t - s > 0 by loop bound */
        /* d must divide n */
        if (n % d != 0)
            continue;
        /* n/d must be ≤ s */
        if (n / d <= s) {
            ++count;
        }
    }
    return count;
}

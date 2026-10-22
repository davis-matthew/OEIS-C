/*
 * A353157  a(n) is the distance from n to the nearest integer whose binary
 *          expansion has no common 1-bits with that of n.
 * https://oeis.org/A353157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = min(n - floor_disjoint(n), ceil_disjoint(n) - n) where
 *            floor_disjoint(n) is the largest m ≤ n with (m & n) == 0, and
 *            ceil_disjoint(n) is the smallest m ≥ n with (m & n) == 0
 * Range:     u8 n <= 638, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0)
        return 0;

    /* cast the index to the sequence type once */
    const seq_t mask = (seq_t)n;

    /* search downwards for the largest compatible number */
    seq_t m_low = n;
    while (m_low && ((m_low & mask) != 0)) {
        --m_low;
    }
    const seq_t diff_low = (seq_t)n - m_low;

    /* search upwards for the smallest compatible number */
    seq_t m_high = n;
    while ((m_high & mask) != 0) {
        ++m_high;
    }
    const seq_t diff_high = m_high - (seq_t)n;

    /* return the smaller distance */
    return (diff_low < diff_high) ? diff_low : diff_high;
}

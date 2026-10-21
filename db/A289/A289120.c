/*
 * A289120  a(n) is the number of odd integers divisible by 7 in ]2*(n-1)^2,
 *          2*n^2[.
 * https://oeis.org/A289120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the number of odd integers divisible by 7 in the open
 *            interval (2*(n-1)^2, 2*n^2)
 * Range:     u8 n <= 893, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* Use 128‑bit intermediates to avoid overflow for large n */
    unsigned __int128 nn   = (unsigned __int128)n;
    unsigned __int128 n1   = nn - 1u;
    unsigned __int128 low  = 2u * n1 * n1;          /* 2*(n-1)^2   */
    unsigned __int128 high = 2u * nn * nn;          /* 2*n^2       */

    /* Smallest odd m with 7*m > low */
    unsigned __int128 m_min = low / 7u + 1u;
    if ((m_min & 1u) == 0u) ++m_min;

    /* Largest odd m with 7*m < high */
    if (high <= 7u) return 0;            /* No odd multiple of 7 fits */
    unsigned __int128 m_max = (high - 1u) / 7u;
    if (m_max == 0u) return 0;
    if ((m_max & 1u) == 0u) --m_max;

    if (m_min > m_max) return 0;

    unsigned __int128 cnt = (m_max - m_min) / 2u + 1u;
    return (seq_t)cnt;
}

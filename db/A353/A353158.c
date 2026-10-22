/*
 * A353158  a(n) is the distance from n to the nearest integer that can be
 *          added to n without carries in base 3.
 * https://oeis.org/A353158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 598, u16 n <= 9841, u32 n <= 9841, u64 n <= 9841,
 *            u128 n <= 9841, big any n
 * Helpers:   carry_free_add
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = min_{m∈ℕ} |n-m|   subject to   n+m has no carries in base 3.
 * The function tests m=n and then increments the distance d until a carry‑free m is found.
 * If d = 0 the distance is 0, otherwise the first d for which either n−d or n+d
 * satisfies the carry‑free condition is returned.
 */

#include <stdbool.h>
#include "../utils/carry_free_add.h"

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;

    /* Distance 0: test whether n itself is carry‑free when added to n. */
    if (carry_free_add(nseq, nseq))
        return 0;

    /* Search increasing distances d.  At most n steps are needed
     * because m = 0 is always allowed. */
    for (idx_t d = 1; d <= n; ++d) {
        /* Check m = n−d.  d ≤ n guarantees no underflow. */
        seq_t m_down = nseq - (seq_t)d;
        if (carry_free_add(nseq, m_down))
            return (seq_t)d;

        /* Check m = n+d.  Skip if addition overflows seq_t. */
        seq_t m_up = nseq + (seq_t)d;
        if (m_up >= nseq && carry_free_add(nseq, m_up))
            return (seq_t)d;
    }

    /* Fallback: the only guaranteed carry‑free number is 0, so
     * distance equals n (converted to seq_t). */
    return (seq_t)n;
}

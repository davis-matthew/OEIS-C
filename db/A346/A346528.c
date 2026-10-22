/*
 * A346528  Distance between first appearance and last appearance of n in
 *          A105224, counting both end points.
 * https://oeis.org/A346528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A346527(n) - A346522(n) + 1 (distance between first and
 *            last appearance of n in A105224, counting both end points)
 * Range:     u8 n <= 5, u16 n <= 140, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    idx_t m = 0;
    seq_t minStart = (seq_t)-1;  /* maximum value */
    seq_t maxEnd   = 0;
    bool found = false;

    while (1) {
        seq_t m_seq = (seq_t)m;                /* m  (seq_t) */
        seq_t v_seq = m_seq + (seq_t)n;        /* v = m + n */

        /* lower bound intersections */
        seq_t lo1 = (v_seq * v_seq + 1) / 2;   /* ceil(v^2/2)   */
        seq_t lo2 = m_seq * m_seq + 1;         /* m^2 + 1       */
        seq_t lower = lo1 > lo2 ? lo1 : lo2;   /* max(lo1,lo2) */

        /* upper bound intersections */
        seq_t hi1 = ((v_seq + 1) * (v_seq + 1) - 1) / 2; /* floor(((v+1)^2-1)/2) */
        seq_t hi2 = (m_seq + 1) * (m_seq + 1);           /* (m+1)^2           */
        seq_t upper = hi1 < hi2 ? hi1 : hi2;              /* min(hi1,hi2)       */

        if (lower <= upper) {          /* valid intersection found */
            if (!found) {
                minStart = lower;
                maxEnd   = upper;
                found    = true;
            } else {
                if (lower < minStart) minStart = lower;
                if (upper > maxEnd)  maxEnd   = upper;
            }
        }

        /* Break when the left lower bound already exceeds the right upper bound.
           For larger m the left bound grows faster, making further intersections
           impossible. */
        if (lo2 > hi1) break;

        ++m;
    }

    /* By construction a(1) exists; thus found will always be true. */
    return maxEnd - minStart + 1;
}

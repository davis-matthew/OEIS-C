/*
 * A355160  Numbers k such that (fractional part of k^(3/2)) > 1/2.
 * https://oeis.org/A355160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A355160: Numbers k such that the fractional part of k^(3/2) is greater than 1/2.
 * The routine scans k in increasing order and tests the equivalence
 * 4*k^3 > (2*floor_sqrt(k^3)+1)^2
 * which follows from the inequality
 *     frac(k*sqrt(k)) > 1/2   ⇔   4*k^3 > (2*floor_sqrt(k^3)+1)^2.
 * Returns the n-th such k (1‑based indexing).
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    idx_t      k;
    seq_t      result = 0;
    seq_t      kcube, s, lhs, rhs, two_s_plus1;

    for (k = 1; ; ++k)
    {
        seq_t kseq = (seq_t)k;            /* promote k to seq_t */
        kcube = kseq * kseq * kseq;       /* k^3 */
        s = floor_sqrt(kcube);            /* floor(sqrt(k^3)) */
        lhs = kcube * (seq_t)4;           /* 4*k^3 */
        two_s_plus1 = (s * (seq_t)2) + (seq_t)1;
        rhs = two_s_plus1 * two_s_plus1;  /* (2*s+1)^2 */

        if (lhs > rhs)
        {
            if (n == 1) { result = kseq; break; }
            --n;                                    /* decrement n only when a match is found */
        }
    }
    return result;
}

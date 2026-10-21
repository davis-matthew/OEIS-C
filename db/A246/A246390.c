/*
 * A246390  Nonnegative integers k satisfying sin(k) <= 0 and sin(k+1) <= 0.
 * https://oeis.org/A246390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A246390
 *  a(n): nonnegative integers k with sin(k) <= 0 and sin(k+1) <= 0.
 *  This is equivalent to k lying in an interval [ceil(mπ), floor((m+1)π)-1]
 *  for an odd integer m.  The function generates these intervals incrementally.
 */

#include <stdint.h>
#include "../utils/floor_pi.h"

seq_t a(idx_t n)
{
    idx_t count = 0;     /* number of terms produced so far (0‑based) */
    idx_t m = 1;         /* current odd m */

    while (1) {
        /* Compute the interval [ceil(mπ), floor((m+1)π)-1] */
        idx_t L = floor_pi(m) + 1;        /* ceil(mπ) because mπ never integer */
        idx_t R = floor_pi(m + 1);        /* floor((m+1)π) */
        idx_t end = R - 1;                /* last k in the block */

        for (idx_t k = L; k <= end; ++k) {
            if (count == n) {
                return (seq_t)k;
            }
            ++count;
        }

        m += 2;   /* next odd m */
    }
}

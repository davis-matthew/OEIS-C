/*
 * A340247  Sum of the third largest parts r of the partitions of n into 4
 *          parts q,r,s,t such that 1 <= q <= r <= s <= t and q + r + s > t.
 * https://oeis.org/A340247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Sum of the third largest parts r of the partitions of n into * 4
 *            parts q,r,s,t such that 1 ≤ q ≤ r ≤ s ≤ t and q + r + s > t. *
 *            (OEIS A340247)
 * Range:     u8 n <= 23, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 4 there are no 4‑part partitions with all parts ≥ 1. */
    if (n < 4)
        return 0;

    idx_t half = n / 2;                 /* floor(n/2) */
    seq_t total = 0;

    /* r is the third largest part, its maximum value is floor((n-1)/3). */
    for (idx_t r = 1; r <= (n - 1) / 3; ++r) {
        /* s cannot be larger than floor((n-1-r)/2). */
        for (idx_t s = r; s <= (n - 1 - r) / 2; ++s) {
            /* t = n - q - r - s  and must satisfy t ≥ s.
               This is equivalent to q + r + 2s ≤ n.  */
            idx_t tr = r + 2 * s;
            if (tr > n)
                continue;          /* no feasible q */

            idx_t U = n - tr;        /* maximal q allowed by t ≥ s */
            if (U > r)
                U = r;               /* plus the ordering q ≤ r */

            /* The inequality q + r + s > n/2 gives a lower bound on q. */
            idx_t sum2 = r + s;
            idx_t lower;
            if (sum2 > half)
                lower = 1;
            else
                lower = half - sum2 + 1;
            if (lower < 1)
                lower = 1;

            if (lower > U)
                continue;          /* no q satisfies all constraints */

            idx_t cnt = U - lower + 1;   /* number of admissible q */
            total += (seq_t)r * (seq_t)cnt;
        }
    }
    return total;
}

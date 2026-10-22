/*
 * A391214  Number of divisors d of n such that d^2 - 4 is relatively prime to
 *          n.
 * https://oeis.org/A391214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors d of n such that gcd(d^2−4 , n) = 1
 * Range:     u8 n <= 26, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

/*  seq_t is an unsigned integer type (8–128 bits or arbitrary precision) ;
    idx_t is a 64‑bit unsigned index. */
seq_t a(idx_t n)
{
    /* Handle n = 0 (outside the offset) */
    if (n == 0) return 0;

    seq_t count = 0;          /* result */
    seq_t sn = n;             /* n as seq_t for gcd */

    /* Enumerate divisors up to sqrt(n) */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* Check divisor d */
            seq_t d_seq = d;
            seq_t v = d_seq * d_seq;
            seq_t val = (v >= 4) ? v - 4 : 4 - v;
            if (gcd(val, sn) == 1)
                ++count;

            /* Check paired divisor n/d, if distinct */
            idx_t od = n / d;
            if (od != d) {
                seq_t od_seq = od;
                seq_t vo = od_seq * od_seq;
                seq_t val2 = (vo >= 4) ? vo - 4 : 4 - vo;
                if (gcd(val2, sn) == 1)
                    ++count;
            }
        }
    }

    return count;
}

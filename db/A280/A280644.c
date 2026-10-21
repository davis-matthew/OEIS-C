/*
 * A280644  Numbers k such that k^3 has an odd number of digits and the middle
 *          digit is 4.
 * https://oeis.org/A280644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n-th natural number k such that k³ has an odd number
 *            of decimal digits and its middle digit is 4
 * Range:     u8 none, u16 n <= 1, u32 n <= 1503, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"      /* seq_t pow10(idx_t e); */
#include "../utils/digit_len.h"  /* int digit_len(seq_t x); */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1, n>=1 is assumed */

    idx_t   found = 0;
    seq_t   k = 1;

    while (found < n) {
        seq_t cube = k * k * k;         /* compute k³ */

        int digits = digit_len(cube);   /* number of decimal digits */
        if (digits % 2 == 1) {          /* odd length? */
            idx_t   mid_pos = (idx_t)((digits - 1) / 2); /* 0‑based */
            seq_t  divisor = pow10(mid_pos);            /* 10^mid_pos */
            seq_t  middle = (cube / divisor) % (seq_t)10;

            if (middle == (seq_t)4) {   /* middle digit is 4? */
                ++found;
                if (found == n)
                    return k;
            }
        }
        ++k;
    }

    return 0;   /* unreachable for n >= 1 */
}

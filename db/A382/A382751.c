/*
 * A382751  Numbers k for which the 3-adic valuation A007949(k) == 0 (mod 3).
 * https://oeis.org/A382751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 176, u16 n <= 6923, u32 n <= 6923, u64 n <= 6923,
 *            u128 n <= 6923, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A382751: k such that v3(k) % 3 == 0.  */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* safety for n < 1 (not used) */

    seq_t k = 1;
    idx_t cnt = 0;

    while (1) {
        seq_t t = k;
        idx_t e = 0;

        /* Count exponent of 3 in k */
        while (t % 3 == 0) {
            t /= 3;
            ++e;
        }

        if (e % 3 == 0) {
            ++cnt;
            if (cnt == n) return k;
        }
        ++k;
    }
}

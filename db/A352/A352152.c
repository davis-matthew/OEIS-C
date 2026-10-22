/*
 * A352152  Reverse each run of consecutive nonzero digits in the decimal
 *          expansion of n.
 * https://oeis.org/A352152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is obtained by reversing each maximal run of consecutive *
 *            nonzero decimal digits of n, keeping zero digits and their
 *            positions. * a(0) = 0. This defines a self‑inverse permutation
 *            of the nonnegative integers * with the property a(10*n) =
 *            10*a(n)
 * Range:     u8 n <= 112, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n = 0 is trivial */
    if (n == 0)
        return 0;

    /* Store digits of n in a temporary array (least significant first) */
    unsigned char buf[64];      /* enough for >20 decimal digits of a 64‑bit value */
    int len = 0;

    idx_t tmp = n;
    while (tmp > 0) {
        buf[len++] = tmp % 10U;    /* store least significant digit */
        tmp /= 10U;
    }

    /* Reverse buf to obtain most‑significant‑digit first order */
    for (int i = 0; i < len / 2; ++i) {
        unsigned char t = buf[i];
        buf[i] = buf[len - 1 - i];
        buf[len - 1 - i] = t;
    }

    /* Build result digit by digit, reversing non‑zero runs */
    seq_t res = 0;
    int i = 0;
    while (i < len) {
        if (buf[i] == 0) {
            res = res * 10U + 0;
            ++i;
        } else {
            /* start of a nonzero run */
            int run_start = i;
            while (i < len && buf[i] != 0)
                ++i;
            int run_end = i - 1;
            /* copy the run in reverse order */
            for (int j = run_end; j >= run_start; --j)
                res = res * 10U + buf[j];
        }
    }

    return res;
}

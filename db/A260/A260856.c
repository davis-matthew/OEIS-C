/*
 * A260856  Base-6 representation is the concatenation of the base-6
 *          representations of 1, 2, ..., n, n-1, ..., 1.
 * https://oeis.org/A260856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the decimal value obtained by concatenating the base‑6 *
 *            representations of 1,2,…,n,n‑1,…,1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t value = 0;
    unsigned char buf[64];
    unsigned int dlen;

    /* forward part: 1 … n */
    for (idx_t i = 1; i <= n; ++i) {
        idx_t t = i;
        dlen = 0;
        while (t > 0) {
            buf[dlen++] = (unsigned char)(t % 6);
            t /= 6;
        }
        /* output most significant digit first */
        for (int j = (int)dlen - 1; j >= 0; --j)
            value = value * 6 + buf[j];
    }

    /* reverse part: n‑1 … 1 */
    if (n > 1) {
        for (idx_t i = n - 1; i; --i) {
            idx_t t = i;
            dlen = 0;
            while (t > 0) {
                buf[dlen++] = (unsigned char)(t % 6);
                t /= 6;
            }
            for (int j = (int)dlen - 1; j >= 0; --j)
                value = value * 6 + buf[j];
        }
    }

    return value;
}

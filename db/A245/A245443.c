/*
 * A245443  Permutation of nonnegative integers: a(n) = A165199(A193231(n)).
 * https://oeis.org/A245443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A165199(A193231(n)); A193231(n) = XOR of (x+1)^e over set
 *            bits e of n (GF(2) polynomials), A165199(m) = m XOR (bits at
 *            positions len-2, len-4, ... of m)
 * Range:     u8 n <= 255, u16 n <= 1023, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Validated: standard, pass, 89 terms through n = 975 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 0;
    seq_t pw = 1;
    idx_t t = n;
    int len = 0;

    while (t) {
        if (t & 1)
            res ^= pw;
        pw = (pw << 1) ^ pw;
        t >>= 1;
        len++;
    }
    /* bit length of res equals bit length of n */
    int pos = len - 2;
    while (pos >= 0) {
        res ^= ((seq_t)1 << pos);
        pos -= 2;
    }
    return res;
}

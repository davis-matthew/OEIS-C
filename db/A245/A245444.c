/*
 * A245444  Permutation of nonnegative integers: a(n) = A193231(A165199(n)).
 * https://oeis.org/A245444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A193231(A165199(n)); A165199 flips every second bit of n
 *            from the second-most significant bit down; A193231(m) = XOR over
 *            set bits e of m of (x+1)^e over GF(2)
 * Range:     u8 n <= 255, u16 n <= 1023, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Validated: standard, pass, 90 terms through n = 1017 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t m = n;
    if (n != 0) {
        int len = 0;
        idx_t t = n;
        while (t) { len++; t >>= 1; }
        int pos = len - 2;
        while (pos >= 0) {
            m ^= ((idx_t)1 << pos);
            pos -= 2;
        }
    }
    seq_t res = 0;
    seq_t mask = 1;
    while (m) {
        if (m & 1)
            res ^= mask;
        mask = (mask << 1) ^ mask;
        m >>= 1;
    }
    return res;
}

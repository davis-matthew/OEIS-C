/*
 * A257799  Parity of binary weight of each term in the infinite trunk of
 *          inverted binary beanstalk: a(n) = A010060(A233271(n)).
 * https://oeis.org/A257799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A010060(A233271(n)); A233271: b(0)=0, b(k+1) = b(k) + 1 +
 *            (number of 0 bits in b(k), none for 0)
 * Range:     u8 n <= 68, u16 n <= 8727, u32 n <= 8727, u64 n <= 8727,
 *            u128 n <= 8727, big any n
 * Validated: standard, pass, 140 terms through n = 8282 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A257799_trunk(idx_t n)
{
    seq_t b = 0;
    for (idx_t i = 0; i < n; ++i) {
        seq_t t = b;
        seq_t zeros = 0;
        while (t) {
            if ((t & 1) == 0) zeros++;
            t >>= 1;
        }
        b = b + 1 + zeros;
    }
    return b;
}

seq_t a(idx_t n)
{
    seq_t x = A257799_trunk(n);
    seq_t res = 0;
    while (x) {
        res ^= 1;
        x &= x - 1;
    }
    return res;
}

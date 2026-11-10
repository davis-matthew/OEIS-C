/*
 * A379471  Composition of bijective bit-reverse and Blue code, in this order:
 *          a(n) = A193231(A057889(n)).
 * https://oeis.org/A379471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A193231(A057889(n)); A057889 reverses the binary digits
 *            of the odd part of n and keeps the trailing zeros, A193231 XORs
 *            (x+1)^e over GF(2) for each set bit e
 * Range:     u8 n <= 255, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 96 terms through n = 9896 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A379471_rev(idx_t n)
{
    if (n == 0) return 0;
    unsigned tz = 0;
    while ((n & 1) == 0) { n >>= 1; ++tz; }
    idx_t rev = 0;
    while (n) {
        rev = (rev << 1) | (n & 1);
        n >>= 1;
    }
    return (seq_t)rev << tz;
}

seq_t a(idx_t n)
{
    idx_t t = n;
    if (t == 0) return 0;
    unsigned tz = 0;
    while ((t & 1) == 0) { t >>= 1; ++tz; }
    idx_t r = 0;
    while (t) {
        r = (r << 1) | (t & 1);
        t >>= 1;
    }
    /* reversed value is r << tz; apply Blue code over seq_t */
    seq_t res = 0;
    seq_t mask = 1;
    seq_t v = (seq_t)r;
    unsigned shift = tz;
    /* bits of v shifted left by tz: skip the first tz zero bits by advancing mask */
    for (unsigned i = 0; i < shift; ++i)
        mask = (mask << 1) ^ mask;
    while (v != 0) {
        if ((v & 1) != 0)
            res ^= mask;
        mask = (mask << 1) ^ mask;
        v >>= 1;
    }
    return res;
}

/*
 * A280501  "Blue kernel" of n: a(n) = A091255(n, A193231(n)).
 * https://oeis.org/A280501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = A091255(n, A193231(n)) = gcd over GF(2)[x] of the
 *            polynomials n and A193231(n) (binary-coded)
 * Range:     u8 n <= 257, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 128 terms through n = 8596 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A193231_blue(idx_t n)
{
    seq_t res = 0;
    seq_t mask = 1;
    idx_t t = n;
    while (t) {
        if (t & 1)
            res ^= mask;
        mask = (mask << 1) ^ mask;
        t >>= 1;
    }
    return res;
}

static unsigned A091255_deg(seq_t p)
{
    unsigned d = 0;
    while (p > 1) {
        p >>= 1;
        d++;
    }
    return d;
}

static seq_t A091255_mod(seq_t a, seq_t b)
{
    if (b == 0)
        return a;
    unsigned db = A091255_deg(b);
    while (a != 0) {
        unsigned da = A091255_deg(a);
        if (da < db)
            break;
        a ^= (b << (da - db));
    }
    return a;
}

static seq_t A091255_gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t r = A091255_mod(a, b);
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t y = A193231_blue(n);
    return A091255_gcd(x, y);
}

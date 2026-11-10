/*
 * A327857  a(n) = A091255(1+A059905(n), 1+A059906(n)).
 * https://oeis.org/A327857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A091255(1+A059905(n), 1+A059906(n)), i.e. the GF(2)[x]
 *            gcd of the polynomials with binary coefficients 1+(even-position
 *            bits of n) and 1+(odd-position bits of n)
 * Range:     u8 n <= 16384, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 122 terms through n = 9984 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A327857_deg(seq_t p)
{
    int d = -1;
    while (p != 0) {
        p >>= 1;
        d++;
    }
    return d;
}

static seq_t A327857_pgcd(seq_t a, seq_t b)
{
    while (b != 0) {
        int db = A327857_deg(b);
        while (a != 0) {
            int da = A327857_deg(a);
            if (da < db)
                break;
            a ^= (b << (da - db));
        }
        seq_t r = a;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    uint64_t t = (uint64_t)n;
    uint64_t x = 0, y = 0;
    int pos = 0;
    while (t) {
        x |= (t & 1ULL) << pos;
        y |= ((t >> 1) & 1ULL) << pos;
        t >>= 2;
        pos++;
    }
    seq_t px = (seq_t)x + 1;
    seq_t py = (seq_t)y + 1;
    return A327857_pgcd(px, py);
}

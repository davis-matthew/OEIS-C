/*
 * A361168  Number of cusps in Weierstrass curve with discriminant A079896(n)
 *          in moduli space M_4 of compact Riemann surfaces of genus 4.
 * https://oeis.org/A361168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = number of prototypes (a,b,c,e) with D = A079896(n) = e^2
 *            + 4*b*c, b > 0, c > 0, c + e < b, 0 <= a < gcd(b,c),
 *            gcd(a,b,c,e) = 1 (one cusp per splitting prototype); computed by
 *            direct enumeration
 * Range:     u8 n <= 23, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 23 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A361168_gcd(uint64_t x, uint64_t y)
{
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

static bool A361168_is_square(uint64_t x)
{
    uint64_t r = 0;
    while ((r + 1) * (r + 1) <= x) ++r;
    return r * r == x;
}

/* n-th positive integer that is 0 or 1 mod 4 and not a perfect square */
static uint64_t A361168_disc(idx_t n)
{
    idx_t found = 0;
    uint64_t c = 1;
    for (;;) {
        uint64_t m = c % 4;
        if ((m == 0 || m == 1) && !A361168_is_square(c)) {
            ++found;
            if (found == n) return c;
        }
        ++c;
    }
}

seq_t a(idx_t n)
{
    uint64_t D = A361168_disc(n);
    long long Dl = (long long)D;
    long long count = 0;
    long long e;
    long long start = (D % 2 == 0) ? 0 : 1;

    for (e = start; e * e < Dl; e += 2) {
        long long N = (Dl - e * e) / 4;
        int sgn;
        for (sgn = 0; sgn < 2; ++sgn) {
            long long ee = sgn ? -e : e;
            long long b;
            if (sgn == 1 && e == 0) break;
            for (b = 1; b <= N; ++b) {
                long long c, g, t;
                if (N % b) continue;
                c = N / b;
                if (!(c + ee < b)) continue;
                g = (long long)A361168_gcd((uint64_t)b, (uint64_t)c);
                for (t = 0; t < g; ++t) {
                    uint64_t h = A361168_gcd((uint64_t)t, (uint64_t)g);
                    h = A361168_gcd(h, (uint64_t)(ee < 0 ? -ee : ee));
                    if (h == 1) ++count;
                }
            }
        }
    }
    return (seq_t)count;
}

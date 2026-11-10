/*
 * A385434  Triangle of Gaussian binomial coefficients (or q-binomial
 *          coefficients) [n,k] for q = 2, reduced mod 3.
 * https://oeis.org/A385434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) mod 3, where T(r,k) is the Gaussian binomial [r,k]
 *            at q=2, n = r(r+1)/2 + k. Since 2 == -1 (mod 3): T(r,k) = 0 if r
 *            is even and k is odd, else binomial(floor(r/2), floor(k/2)) mod
 *            3 (Lucas)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 77 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static unsigned A385434_binom3(uint64_t a, uint64_t b)
{
    unsigned res = 1;
    while (a > 0 || b > 0) {
        unsigned x = (unsigned)(a % 3), y = (unsigned)(b % 3);
        if (y > x) return 0;
        if (x == 2 && y == 1) res = (res * 2) % 3;
        a /= 3; b /= 3;
    }
    return res;
}

seq_t a(idx_t n)
{
    uint64_t r = (uint64_t)((sqrt(8.0 * (double)n + 1.0) - 1.0) / 2.0);
    while ((unsigned __int128)r * (r + 1) / 2 > (unsigned __int128)n) --r;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= (unsigned __int128)n) ++r;
    uint64_t k = (uint64_t)((unsigned __int128)n - (unsigned __int128)r * (r + 1) / 2);
    if ((r & 1) == 0 && (k & 1) == 1) return (seq_t)0;
    return (seq_t)A385434_binom3(r / 2, k / 2);
}

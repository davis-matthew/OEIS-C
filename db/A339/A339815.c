/*
 * A339815  Let x = A019565(2*n); a(n) is the difference between 2-adic
 *          valuations of phi(x) and (x-1).
 * https://oeis.org/A339815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A007814(A000010(A019565(2n))) - A007814(A019565(2n)-1); x
 *            = A019565(2n) = product of prime(k+2) over set bits k of n,
 *            v2(phi(x)) = sum of v2(p-1)
 * Range:     i8 n <= 8195, i16 n <= 65537, i32 n <= 65537, i64 n <= 65537,
 *            i128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9993 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint32_t A339815_odd_primes[64] = {
    3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53,
    59, 61, 67, 71, 73, 79, 83, 89,
    97, 101, 103, 107, 109, 113, 127, 131,
    137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223,
    227, 229, 233, 239, 241, 251, 257, 263,
    269, 271, 277, 281, 283, 293, 307, 311, 313
};

seq_t a(idx_t n)
{
    seq_t x = 1;
    long long vphi = 0;
    int bit = 0;
    while (n != 0) {
        if (n & 1ULL) {
            uint32_t p = A339815_odd_primes[bit];
            x *= (seq_t)p;
            uint32_t q = p - 1;
            while ((q & 1u) == 0) { vphi++; q >>= 1; }
        }
        n >>= 1;
        bit++;
    }
    seq_t y = x - 1;
    long long vx = 0;
    while (y % 2 == 0) {
        y /= 2;
        vx++;
    }
    return (seq_t)(vphi - vx);
}

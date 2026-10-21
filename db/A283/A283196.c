/*
 * A283196  Number of ways to write n as x^2 + y^2 + z^2 + w^2 with 2*x + y
 *          and 2*x + z both squares, where x,y,z are integers with |y| <=
 *          |z|, and w is a positive integer.
 * https://oeis.org/A283196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of representations of n as x^2 + y^2 + z^2 + w^2
 *            with 2*x + y and 2*x + z both perfect squares, |y| ≤ |z|, and w
 *            > 0. The function enumerates all integer quadruples (x,y,z,w)
 *            satisfying these conditions and counts them. It is efficient
 *            enough for n ≤ 10 000
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   uabs_ll
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/uabs_ll.h"

/* integer square root (floor) for 64‑bit unsigned values */
static inline unsigned long long isqrt_u64(unsigned long long n)
{
    unsigned long long low = 0, high = 4294967295ULL; /* 2^32-1 */
    while (low < high) {
        unsigned long long mid = (low + high + 1) >> 1;
        if (mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset 1; not used for n ≥ 1 */

    unsigned long long N = (unsigned long long)n;
    unsigned long long sqrtN = isqrt_u64(N);
    unsigned long long imax = sqrtN;     /* |x| cannot exceed sqrt(n) */
    seq_t count = 0;

    for (long long x = -(long long)imax; x <= (long long)imax; ++x) {
        unsigned long long x2 = (unsigned long long)(x * x);
        if (x2 > N) continue;
        unsigned long long remX = N - x2;
        if (remX == 0) continue;         /* w^2 must be ≥ 1 */

        unsigned long long maxA = sqrtN + 2 * imax;   /* safe upper bound for a */
        for (unsigned long long a = 0; a <= maxA; ++a) {
            unsigned long long a2 = a * a;
            long long y = (long long)a2 - 2 * x;
            unsigned long long yAbs = uabs_ll(y);
            if (yAbs * yAbs > remX) continue;

            unsigned long long y2 = yAbs * yAbs;
            unsigned long long remXY = remX - y2;
            if (remXY <= 0) continue;   /* at least z^2 + w^2 are needed */

            for (unsigned long long b = 0; b <= maxA; ++b) {
                unsigned long long b2 = b * b;
                long long z = (long long)b2 - 2 * x;
                unsigned long long zAbs = uabs_ll(z);
                if (zAbs * zAbs > remXY) continue;
                if (yAbs > zAbs) continue;    /* enforce |y| ≤ |z| */

                unsigned long long z2 = zAbs * zAbs;
                unsigned long long remZ = remXY - z2;
                if (remZ <= 0) continue;   /* w^2 ≥ 1 required */

                unsigned long long w = isqrt_u64(remZ);
                if (w == 0 || w * w != remZ) continue;   /* w must be a positive square */

                ++count;
            }
        }
    }
    return count;
}

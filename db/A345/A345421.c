/*
 * A345421  Row 4 of array in A345417.
 * https://oeis.org/A345421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = u, the smallest-|u| solution of 7*u + prime(n)*v = 1 (u =
 *            0 if prime(n) = 7), i.e. u = 7^(-1) mod prime(n) reduced to the
 *            range -(p-1)/2..p/2
 * Range:     i8 n <= 64, i16 n <= 64, i32 n <= 64, i64 n <= 64, i128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-10)
 * Helpers:   nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/nth_prime.h"

static int64_t A345421_inv(int64_t a, int64_t m)
{
    int64_t r0 = m, r1 = a % m, t0 = 0, t1 = 1;
    while (r1 != 0) {
        int64_t q = r0 / r1;
        int64_t r2 = r0 - q * r1;
        int64_t t2 = t0 - q * t1;
        r0 = r1; r1 = r2;
        t0 = t1; t1 = t2;
    }
    if (t0 < 0) t0 += m;
    return t0;
}

seq_t a(idx_t n)
{
    int64_t p = (int64_t)nth_prime(n);
    if (p == 7) return (seq_t)0;
    int64_t u = A345421_inv(7, p);
    if (u > p / 2) u -= p;
    return (seq_t)u;
}

/*
 * A248356  Numbers k such that A248355(k+1) = A248355(k).
 * https://oeis.org/A248356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th k such that A248355(k+1) = A248355(k), where
 *            A248355(k) = least j with Pi - j*sin(Pi/j) < 1/(2k); computed by
 *            a monotone linear scan
 * Range:     u8 n <= 207, u16 n <= 9000, u32 n <= 9000, u64 n <= 9000,
 *            u128 n <= 9000, big any n
 * Validated: standard, pass, 81 terms through n = 8133 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

static int A248356_ok(unsigned long long j, unsigned long long k)
{
    static const long double PI = 3.141592653589793238462643383279502884L;
    long double jl = (long double)j;
    long double val = PI - jl * sinl(PI / jl);
    long double thresh = 1.0L / (2.0L * (long double)k);
    return val < thresh;
}

seq_t a(idx_t n)
{
    unsigned long long j = 1;
    unsigned long long count = 0;
    unsigned long long k = 1;
    unsigned long long cur;
    while (!A248356_ok(j, k)) ++j;
    cur = j;
    for (;; ++k) {
        while (!A248356_ok(j, k + 1)) ++j;
        if (j == cur) {
            ++count;
            if (count == (unsigned long long)n)
                return (seq_t)k;
        }
        cur = j;
    }
}

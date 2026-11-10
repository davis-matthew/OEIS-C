/*
 * A332204  a(n) is the real part of f(n) defined by f(0) = 0, and f(n+1) =
 *          f(n) + g((1+i)^(A065359(n) mod 8)) (where g(z) = z/gcd(Re(z),
 *          Im(z)) and i denotes the imaginary unit).
 * https://oeis.org/A332204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Re(f(n)), f(0) = 0, f(m+1) = f(m) + g((1+i)^(A065359(m)
 *            mod 8)), g(z) = z/gcd(Re z, Im z); direct summation of step real
 *            parts
 * Range:     u8 n <= 463, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 84 terms through n = 9548 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A065359_alt(idx_t n)
{
    int res = 0, sign = 1;
    while (n) {
        if (n & 1) res += sign;
        sign = -sign;
        n >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* real parts of g((1+i)^k), k = 0..7 */
    static const int dx[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    long long sum = 0;
    for (idx_t m = 0; m < n; ++m) {
        int k = A065359_alt(m);
        k = ((k % 8) + 8) % 8;
        sum += dx[k];
    }
    return (seq_t)sum;
}

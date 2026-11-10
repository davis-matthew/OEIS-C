/*
 * A372876  Row sums of A372877.
 * https://oeis.org/A372876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A372877(n, k) = Sum_{k=1..n, k odd}
 *            Jacobi(n/k) (row sums of Jacobi's triangle)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 85 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static int A372876_jac(uint64_t a, uint64_t n) {
    if (n == 0) return 0;
    a %= n;
    int res = 1;
    while (a != 0) {
        while ((a & 1) == 0) {
            a >>= 1;
            uint64_t m = n & 7;
            if (m == 3 || m == 5) res = -res;
        }
        uint64_t t = a; a = n; n = t;
        if ((a & 3) == 3 && (n & 3) == 3) res = -res;
        a %= n;
    }
    return (n == 1) ? res : 0;
}

seq_t a(idx_t n)
{
    long long s = 0;
    for (idx_t k = 1; k <= n; k += 2)
        s += A372876_jac((uint64_t)n, (uint64_t)k);
    return (seq_t)s;
}

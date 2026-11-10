/*
 * A245812  Self-inverse permutation of natural numbers: a(0) = 0, a(1) = 1,
 *          and for n > 1, if A065620(n) < 0, a(n) =
 *          A065621(1+a(-(A065620(n)))), otherwise a(n) =
 *          A048724(a(A065620(n)-1)).
 * https://oeis.org/A245812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1; for n>1, with v=A065620(n): if v<0,
 *            a(n)=A065621(1+a(-v)), else a(n)=A048724(a(v-1)); A065620(n) is
 *            the alternating sum of the set bits of n from the lowest bit up
 * Range:     u8 n <= 255, u16 n <= 2047, u32 n <= 2047, u64 n <= 2047,
 *            u128 n <= 2047, big any n
 * Validated: standard, pass, 95 terms through n = 1866 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)n;

    idx_t P = 0, N = 0;
    idx_t x = n;
    int sign = 1;
    while (x != 0) {
        idx_t low = x & (~x + 1);
        if (sign > 0) P += low; else N += low;
        x -= low;
        sign = -sign;
    }

    if (N > P) {
        seq_t m = (seq_t)(N - P);
        seq_t r = a(N - P) + 1;
        (void)m;
        seq_t low = r ^ (r & (r - 1));
        return r ^ ((r - low) << 1);
    } else {
        idx_t k = P - N - 1;
        seq_t r = a(k);
        return r ^ (r << 1);
    }
}

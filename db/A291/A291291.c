/*
 * A291291  Binary sequence defined by Baldini-Eschgfäller coupled dynamical
 *          system (f,lambda,alpha) with f = A291290, lambda(y) = 1-y for y in
 *          Y = {0,1}, and alpha(k) = k mod 2 for k in Omega = {0,1,2,3}.
 * https://oeis.org/A291291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n mod 2 for n < 4, otherwise a(n) = 1 - a(f(n)), where f
 *            = A291290: f(n) = n/3 if n==0 mod 3, floor((n+1)/2) if n==1 mod
 *            3, n-2 otherwise
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 106 terms through n = 105 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A291290_f(idx_t n) {
    idx_t m = n % 3;
    if (m == 0) return n / 3;
    if (m == 1) return (n >> 1) + (n & 1);
    return n - 2;
}

seq_t a(idx_t n) {
    unsigned flips = 0;
    while (n >= 4) {
        n = A291290_f(n);
        flips++;
    }
    unsigned v = (unsigned)(n & 1);
    if (flips & 1) v = 1 - v;
    return (seq_t)v;
}

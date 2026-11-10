/*
 * A265329  Self-inverse permutation of nonnegative integers: a(n) =
 *          A263273(A057889(A263273(n))).
 * https://oeis.org/A265329
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A263273(A057889(A263273(n))), where each is the "adjusted
 *            reverse" (trailing zeros fixed) in base 3, base 2, base 3
 * Range:     u8 n <= 85, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 100 terms through n = 7685 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A265329_adjrev(seq_t n, unsigned base)
{
    if (n == 0) return (seq_t)0;
    seq_t pw = 1;
    seq_t t = n;
    while (t % base == 0) {
        t /= base;
        pw *= base;
    }
    seq_t rev = 0;
    while (t > 0) {
        rev = rev * base + (t % base);
        t /= base;
    }
    return rev * pw;
}

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    x = A265329_adjrev(x, 3);
    x = A265329_adjrev(x, 2);
    x = A265329_adjrev(x, 3);
    return x;
}

/*
 * A304760  Binary encoding of 1-digits in ternary representation of
 *          A254103(n).
 * https://oeis.org/A304760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A289813(A254103(n)); A254103: a(0)=0, a(2n)=3*a(n)-1,
 *            a(2n+1)=floor(3*(1+a(n))/2); A289813: a(0)=0, a(3n)=2*a(n),
 *            a(3n+1)=2*a(n)+1, a(3n+2)=2*a(n)
 * Range:     u8 n <= 63, u16 n <= 8335, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 109 terms through n = 9092 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A254103_a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t r = A254103_a(n >> 1);
    if ((n & 1) == 0)
        return (seq_t)3 * r - (seq_t)1;
    return ((seq_t)3 * ((seq_t)1 + r)) / (seq_t)2;
}

static seq_t A289813_recur(seq_t x)
{
    if (x == 0) return (seq_t)0;
    seq_t r = A289813_recur(x / 3);
    if (x % 3 == 1)
        return (r << 1) | 1;
    return r << 1;
}

seq_t a(idx_t n)
{
    return A289813_recur(A254103_a(n));
}

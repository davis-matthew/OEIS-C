/*
 * A326991  First quadrisection of A133058.
 * https://oeis.org/A326991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A133058(4n), where A133058(0)=A133058(1)=1 and for m>1,
 *            A133058(m) = A133058(m-1)+m+1 if gcd(A133058(m-1),m)=1, else
 *            A133058(m-1)/gcd(A133058(m-1),m)
 * Range:     u8 n <= 13, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 72 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A326991_gcd(seq_t x, seq_t y)
{
    while (y != (seq_t)0) {
        seq_t t = y;
        y = x % y;
        x = t;
    }
    return x;
}

static seq_t A326991_A133058(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;
    seq_t prev = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t g = A326991_gcd(prev, (seq_t)i);
        if (g == (seq_t)1)
            prev = prev + (seq_t)i + (seq_t)1;
        else
            prev = prev / g;
    }
    return prev;
}

seq_t a(idx_t n)
{
    return A326991_A133058(n * 4);
}

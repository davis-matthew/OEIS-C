/*
 * A326993  Third quadrisection of A133058.
 * https://oeis.org/A326993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A133058(4n+2); A133058: a(0)=a(1)=1, a(k)=a(k-1)+k+1 if
 *            gcd(a(k-1),k)=1, else a(k-1)/gcd(a(k-1),k)
 * Range:     u8 n <= 13, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 69 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A133058_gcd(seq_t x, seq_t y)
{
    while (y != (seq_t)0) {
        seq_t t = y;
        y = x % y;
        x = t;
    }
    return x;
}

seq_t a(idx_t n)
{
    idx_t m = n * 4 + 2;
    seq_t prev = (seq_t)1;
    for (idx_t i = 2; i <= m; ++i) {
        seq_t g = A133058_gcd(prev, (seq_t)i);
        if (g == (seq_t)1)
            prev = prev + (seq_t)i + (seq_t)1;
        else
            prev = prev / g;
    }
    return prev;
}

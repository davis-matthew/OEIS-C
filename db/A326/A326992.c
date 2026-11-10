/*
 * A326992  Second quadrisection of A133058.
 * https://oeis.org/A326992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A133058(4n+1), where A133058: b(0)=b(1)=1; for m>1,
 *            b(m)=b(m-1)+m+1 if gcd(b(m-1),m)=1, else
 *            b(m)=b(m-1)/gcd(b(m-1),m)
 * Range:     u8 n <= 13, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 64 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A133058_gcd_local(seq_t x, seq_t y)
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
    idx_t target = n * 4 + 1;
    seq_t prev = (seq_t)1;  /* b(1) */
    for (idx_t i = 2; i <= target; ++i) {
        seq_t g = A133058_gcd_local(prev, (seq_t)i);
        if (g == (seq_t)1)
            prev = prev + (seq_t)i + (seq_t)1;
        else
            prev = prev / g;
    }
    return prev;
}

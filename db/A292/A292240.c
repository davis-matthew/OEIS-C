/*
 * A292240  Binary encoding of 0-digits in ternary representation of
 *          A254103(n).
 * https://oeis.org/A292240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A291770(A254103(n)); A254103: a(0)=0, a(2n)=3*a(n)-1,
 *            a(2n+1)=floor(3*(1+a(n))/2); A291770(m) = binary number whose
 *            1-bits mark the non-leading 0 digits of m in base 3
 * Range:     u8 n <= 63, u16 n <= 16383, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 122 terms through n = 9994 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A254103_a(idx_t n)
{
    seq_t v = 0;
    int top = 63;
    if (n == 0) return 0;
    while (!((n >> top) & 1)) top--;
    for (int i = top; i >= 0; --i) {
        if ((n >> i) & 1) v = ((seq_t)3 * (v + (seq_t)1)) / (seq_t)2;
        else v = (seq_t)3 * v - (seq_t)1;
    }
    return v;
}

static seq_t A291770_a(seq_t x)
{
    seq_t result = 0;
    unsigned pos = 0;
    const unsigned bitlen = (unsigned)sizeof(seq_t) * 8;
    while (x > 0) {
        if (x % 3 == 0) {
            if (pos < bitlen) result |= ((seq_t)1 << pos);
        }
        x /= 3;
        pos++;
    }
    return result;
}

seq_t a(idx_t n)
{
    return A291770_a(A254103_a(n));
}

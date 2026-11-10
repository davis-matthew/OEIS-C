/*
 * A276082  a(0) = 0, a(2n) = A153880(a(n)), a(2n+1) = 1+A255411(a(n)).
 * https://oeis.org/A276082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0, a(2n) = A153880(a(n)), a(2n+1) = 1 + A255411(a(n))
 * Range:     u8 n <= 18, u16 n <= 128, u32 n <= 2302, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 82 terms through n = 7189 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* A153880: shift factorial base representation left by one digit */
static seq_t A153880_f(seq_t x)
{
    seq_t result = 0;
    seq_t fac = 2;
    unsigned long i = 1;
    while (x > 0) {
        seq_t d = x % (seq_t)(i + 1);
        result += d * fac;
        x /= (seq_t)(i + 1);
        fac *= (seq_t)(i + 2);
        ++i;
    }
    return result;
}

/* A255411: shift left, increment nonzero digits by one */
static seq_t A255411_f(seq_t x)
{
    seq_t result = 0;
    seq_t fact = 2;            /* (k+2)! for digit k */
    unsigned long k = 0;
    while (x > 0) {
        seq_t d = x % (seq_t)(k + 2);
        x /= (seq_t)(k + 2);
        if (d != 0) result += (d + 1) * fact;
        fact *= (seq_t)(k + 3);
        ++k;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t v = 0;
    int top = 63;
    while (top >= 0 && !((n >> top) & 1)) --top;
    for (int b = top; b >= 0; --b) {
        if ((n >> b) & 1) v = (seq_t)1 + A255411_f(v);
        else v = A153880_f(v);
    }
    return v;
}

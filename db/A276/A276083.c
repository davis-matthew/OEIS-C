/*
 * A276083  a(0) = 0, a(2n) = A255411(a(n)), a(2n+1) = 1+A153880(a(n)).
 * https://oeis.org/A276083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0, a(2n) = A255411(a(n)), a(2n+1) = 1 + A153880(a(n));
 *            computed by scanning the bits of n from the most significant
 *            down
 * Range:     u8 n <= 15, u16 n <= 127, u32 n <= 2047, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 82 terms through n = 7231 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A255411_a(seq_t n)
{
    unsigned char dig[100];
    unsigned int k = 0;
    seq_t rem = n;
    while (rem > 0) {
        unsigned int div = k + 2;
        dig[k] = (unsigned char)(rem % div);
        rem /= div;
        ++k;
    }
    seq_t result = 0;
    seq_t fact = 1;
    for (unsigned int i = 1; i <= k + 1; ++i) {
        if (i > 1 && dig[i - 2] != 0) {
            result += (seq_t)(dig[i - 2] + 1) * fact;
        }
        fact *= (seq_t)(i + 1);
    }
    return result;
}

static seq_t A153880_a(seq_t n)
{
    seq_t result = 0;
    seq_t fac = 2;
    unsigned int i = 1;
    while (n > 0) {
        seq_t d = n % (i + 1);
        result += d * fac;
        n = (n - d) / (i + 1);
        fac *= (seq_t)(i + 2);
        ++i;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t v = 0;
    if (n == 0) return v;
    int top = 63;
    while (!((n >> top) & 1)) --top;
    for (int b = top; b >= 0; --b) {
        if ((n >> b) & 1) {
            v = A153880_a(v) + 1;
        } else {
            v = A255411_a(v);
        }
    }
    return v;
}

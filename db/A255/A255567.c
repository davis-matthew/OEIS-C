/*
 * A255567  a(1) = 1, a(2) = 2, after which, a(2n+1) = 1 + a(2n), a(2n) =
 *          A255411(a(n)).
 * https://oeis.org/A255567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 2, a(2n+1) = 1 + a(2n), a(2n) = A255411(a(n))
 * Range:     u8 n <= 15, u16 n <= 127, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Validated: standard, pass, 79 terms through n = 966 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A255411_a(seq_t n)
{
    unsigned char dig[512];
    unsigned int k = 0;
    seq_t rem = n;
    while (rem > 0) {
        unsigned int d = k + 2;
        dig[k] = (unsigned char)(long long)(rem % d);
        rem /= d;
        ++k;
    }
    seq_t result = 0;
    seq_t fact = 1;
    for (unsigned int i = 1; i <= k + 1; ++i) {
        if (i > 1 && dig[i - 2] != 0) {
            result += (seq_t)((unsigned int)dig[i - 2] + 1) * fact;
        }
        fact *= (seq_t)(i + 1);
    }
    return result;
}

static seq_t A255567_rec(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n & 1) return A255567_rec(n - 1) + 1;
    return A255411_a(A255567_rec(n / 2));
}

seq_t a(idx_t n)
{
    return A255567_rec(n);
}

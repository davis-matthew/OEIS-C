/*
 * A262559  A230030(n) written in base 5.
 * https://oeis.org/A262559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A230030(n) written in base 5 (digits read as a decimal
 *            number); A230030(n) = P(5^(n-2)) for n >= 6, P(x) = 25x^4 +
 *            15x^3 - 4x^2 + 3x + 1, first five terms constants
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 11 (2026-11-07)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

static seq_t A230030_a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)972799;
    if (n == 3) return (seq_t)3051273374LL;
    if (n == 4) return (seq_t)6132750376LL;
    if (n == 5) return (seq_t)839228035909LL;

    seq_t x  = ipow((seq_t)5, (seq_t)(n - 2));
    seq_t x2 = x * x;
    seq_t x3 = x2 * x;
    seq_t x4 = x3 * x;

    seq_t term = (seq_t)25 * x4;
    term += (seq_t)15 * x3;
    term += (seq_t)3 * x;
    term += (seq_t)1;
    term -= (seq_t)4 * x2;
    return term;
}

seq_t a(idx_t n)
{
    seq_t x = A230030_a(n);
    seq_t r = 0;
    seq_t p = 1;
    while (x > 0) {
        seq_t d = x % 5;
        r += d * p;
        p *= 10;
        x /= 5;
    }
    return r;
}

/*
 * A356571  a(n) = floor(f(n)), where f(n) = n^4*(15-24*n+10*n^2) +
 *          20*n^5*(1-n)^3 / (1-2*n(1-n)).
 * https://oeis.org/A356571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ⌊ n⁴·(15 − 24·n + 10·n²) + 20·n⁵·(1 − n)³ / (1 − 2·n·(1 −
 *            n)) ⌋ * Computation uses only integer arithmetic, no
 *            floating‑point operations
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 10, i64 n <= 30, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t floor_div(seq_t a, seq_t b)
{
    /* b > 0 always for this sequence */
    seq_t q = a / b;          /* truncates toward zero */
    seq_t r = a % b;          /* same sign as a */
    if (r != (seq_t)0 && a < (seq_t)0) q -= (seq_t)1;
    return q;
}

seq_t a(idx_t n)
{
    /* cast the index to seq_t once */
    seq_t N = (seq_t)n;

    /* n², n⁴ */
    seq_t n2 = N * N;
    seq_t n4 = n2 * n2;

    /* S = n⁴ * (15 - 24·n + 10·n²) */
    seq_t tmp = (seq_t)15 - (seq_t)24 * N + (seq_t)10 * n2;
    seq_t S = n4 * tmp;

    /* (1 - n)³ */
    seq_t diff = (seq_t)1 - N;
    seq_t diff2 = diff * diff;
    seq_t diff3 = diff2 * diff;

    /* n⁵ = n⁴ · n */
    seq_t n5 = n4 * N;

    /* numerator of the fractional part: 20·n⁵·(1 - n)³ */
    seq_t num = (seq_t)20 * n5 * diff3;

    /* denominator: 1 - 2·n·(1 - n) */
    seq_t denom = (seq_t)1 - (seq_t)2 * N * diff;

    /* floor division for the fractional part */
    seq_t frac = floor_div(num, denom);

    /* final value */
    return S + frac;
}

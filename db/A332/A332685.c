/*
 * A332685  a(n) = Sum_{k=1..n} mu(k/gcd(n, k)).
 * https://oeis.org/A332685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} mu(k/gcd(n,k))
 * Range:     i8 n <= 299, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"

/* Euclidean algorithm for unsigned 64‑bit integers (works in C and C++). */
static inline idx_t gcd_u64(idx_t a, idx_t b)
{
    while (b)
    {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* n is defined for n >= 1; behaviour for n = 0 is irrelevant */
    if (n == 0)
        return (seq_t)0;

    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t g = gcd_u64(k, n);
        idx_t d = k / g;
        sum += (seq_t)mobius(d);   /* mobius returns -1, 0, or 1 */
    }
    return sum;
}

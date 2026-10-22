/*
 * A368744  a(n) = Sum_{d|n} (-1)^(d+1)*phi(d), where phi(n) = A000010(n).
 * https://oeis.org/A368744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Sum_{d|n} (-1)^(d+1)*phi(d)
 * Range:     i8 n <= 128, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Simple integer power helper */
static inline seq_t pow_seq(seq_t base, uint64_t exp)
{
    seq_t res = 1;
    while (exp) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;

    uint64_t tmp = (uint64_t)n;
    seq_t result = 1;

    /* factor 2 separately */
    uint64_t e = 0;
    while ((tmp & 1ULL) == 0 && tmp > 0) {
        tmp >>= 1;
        e++;
    }
    if (e > 0) {
        seq_t pow2 = pow_seq((seq_t)2, e);
        result *= (seq_t)(2 - pow2);
    }

    /* trial division for odd factors */
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                e++;
            }
            seq_t term = pow_seq((seq_t)p, e);
            result *= term;
        }
    }

    /* remaining prime factor, if any */
    if (tmp > 1) {
        seq_t term = pow_seq((seq_t)tmp, 1);
        result *= term;
    }

    return result;
}

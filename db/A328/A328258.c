/*
 * A328258  a(n) = Sum_{d|n, gcd(d,n/d) = 1} (-1)^(d + 1) * d.
 * https://oeis.org/A328258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, look
 * Formula:   a(n) = Sum_{d|n, gcd(d,n/d)=1} (-1)^(d+1) * d Equivalent product
 *            formula: a(n) = ∏_{p^k‖n} (1 - (-1)^p * p^k)
 * Range:     i8 n <= 92, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;        /* a(1) = 1 */

    seq_t result = 1;
    idx_t tmp = n;

    /* Handle factor 2 separately (p even -> (-1)^p = 1) */
    if (tmp % 2 == 0) {
        idx_t cnt = 0;
        while (tmp % 2 == 0) {
            tmp /= 2;
            ++cnt;
        }
        seq_t pk = 1;
        for (idx_t i = 0; i < cnt; ++i) pk *= (seq_t)2;
        seq_t term = (seq_t)1 - pk;          /* 1 - 1 * 2^k */
        result *= term;
    }

    /* Handle odd prime factors */
    for (idx_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            idx_t cnt = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++cnt;
            }
            seq_t pk = 1;
            for (idx_t i = 0; i < cnt; ++i) pk *= (seq_t)p;
            seq_t term = (seq_t)1 + pk;      /* 1 - (-1) * p^k = 1 + p^k  (p odd) */
            result *= term;
        }
    }

    /* Remaining cofactor, if prime */
    if (tmp > 1) {
        seq_t pk = (seq_t)tmp;
        seq_t term = (seq_t)1 + pk;          /* tmp is odd prime */
        result *= term;
    }

    return result;
}

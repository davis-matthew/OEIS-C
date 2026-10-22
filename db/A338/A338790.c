/*
 * A338790  a(n) = rad(n)^2 - sigma(n), where rad(n) is the squarefree kernel
 *          of n (A007947) and sigma(n) is the sum of divisors of n (A000203).
 * https://oeis.org/A338790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = rad(n)^2 - sigma(n) (OEIS A338790)
 * Range:     i8 n <= 12, i16 n <= 181, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by naive factorisation; suitable for n up to ~10000. */

#include <stdint.h>

/* Compute the sum of divisors sigma(n). */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = 1;
    idx_t m = n;
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            seq_t term = 1;
            seq_t p_pow = 1;
            while (m % p == 0) {
                m /= p;
                p_pow *= (seq_t)p;
                term += p_pow;
            }
            sum *= term;
        }
    }
    if (m > 1) {              /* remaining prime factor */
        sum *= (seq_t)(1 + m);
    }
    return sum;
}

/* Compute the square‑free kernel rad(n). */
static seq_t rad_of(idx_t n)
{
    seq_t rad = 1;
    idx_t m = n;
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            rad *= (seq_t)p;
            while (m % p == 0) {
                m /= p;
            }
        }
    }
    if (m > 1) {              /* remaining prime factor */
        rad *= (seq_t)m;
    }
    return rad;
}

seq_t a(idx_t n)
{
    seq_t r = rad_of(n);
    seq_t s = sigma_of(n);
    return r * r - s;
}

/*
 * A359577  Dirichlet inverse of A085731, where A085731 is the greatest common
 *          divisor of n and the arithmetic derivative of n.
 * https://oeis.org/A359577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   not stated
 * Range:     i8 n <= 1322, i16 n <= 9973, i32 n <= 9973, i64 n <= 9973,
 *            i128 n <= 9973, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dirichlet inverse of A085731 (gcd(n, arithmetic derivative of n)).
   a(1) = 1; for n > 1:
   a(n) = -∑_{d|n, d<n} A085731(n/d) * a(d).  Multiplicative.  */
#include <stdint.h>

/* power function: p^e  (in terms of seq_t) */
static seq_t pow_p(idx_t p, unsigned e)
{
    seq_t v = 1;
    for (unsigned i = 0; i < e; ++i)
        v *= (seq_t)p;
    return v;
}

/* Dirichlet inverse on the prime‑power level: a(p^k) */
static seq_t a_prime_power(idx_t p, unsigned k)
{
    seq_t a[65];              /* 65 ≧ maximal exponent for 64‑bit n */
    a[0] = (seq_t)1;
    for (unsigned e = 1; e <= k; ++e) {
        seq_t sum = 0;
        for (unsigned i = 0; i < e; ++i) {
            unsigned rem = e - i;
            seq_t gval;
            if (rem % p == 0)
                gval = pow_p(p, rem);          /* g(p^rem) = p^rem  if p | rem */
            else
                gval = pow_p(p, rem - 1);      /* otherwise g(p^rem) = p^(rem-1) */
            sum += a[i] * gval;
        }
        a[e] = -sum;
    }
    return a[k];
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t tmp = n;
    idx_t p = 2;

    /* trial‑division factorisation */
    while (p * p <= tmp) {
        if (tmp % p == 0) {
            unsigned exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            result *= a_prime_power(p, exp);
        }
        p = (p == 2) ? 3 : p + 2;   /* skip even numbers after 2 */
    }

    if (tmp > 1)               /* remaining prime factor greater than sqrt(n) */
        result *= a_prime_power(tmp, 1);

    return result;
}

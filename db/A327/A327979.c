/*
 * A327979  a(n) = gcd(n, A002322(n)), where A002322 is Carmichael lambda,
 *          also known as psi.
 * https://oeis.org/A327979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, A002322(n)) // Carmichael lambda
 * Range:     u8 n <= 1022, u16 n <= 9635, u32 n <= 9635, u64 n <= 9635,
 *            u128 n <= 9635, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = gcd(n, λ(n))  where λ is Carmichael's function.  */
#include "../utils/gcd.h"
#include <stdint.h>

/* Helper: Carmichael λ for a prime power p^e
   Assumes p is odd or p==2. Returns λ(p^e). */
static seq_t lambda_prime_power(idx_t p, idx_t e)
{
    if (p == 2) {
        if (e == 1) return (seq_t)1;
        if (e == 2) return (seq_t)2;
        /* e >= 3 */
        return (seq_t)1 << (e - 2);
    } else {
        /* λ(p^e) = φ(p^e) = p^(e-1) * (p-1)   (odd prime) */
        seq_t pow = (seq_t)1;
        for (idx_t i = 0; i < e - 1; ++i)
            pow *= (seq_t)p;
        return pow * (seq_t)(p - 1);
    }
}

/* Helper: least common multiple using gcd  */
static seq_t lcm_seq(seq_t a, seq_t b)
{
    seq_t g = gcd(a, b);
    return a / g * b;
}

/* Carmichael λ(n) for n >= 1 */
static seq_t carmichael_lambda(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t lambda = (seq_t)1;
    idx_t t = n;
    for (idx_t p = 2; (idx_t)p * p <= t; ++p) {
        if (t % p == 0) {
            idx_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            seq_t part = lambda_prime_power(p, e);
            lambda = lcm_seq(lambda, part);
        }
    }
    if (t > 1) {              /* remaining prime factor */
        seq_t part = lambda_prime_power(t, 1);
        lambda = lcm_seq(lambda, part);
    }
    return lambda;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    seq_t lambda = carmichael_lambda(n);
    return gcd((seq_t)n, lambda);
}

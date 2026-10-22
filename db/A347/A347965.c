/*
 * A347965  Dirichlet convolution of A003415 (arithmetic derivative) with
 *          A003961 (prime shift towards larger primes).
 * https://oeis.org/A347965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A003415(d) * A003961(n/d)
 * Range:     u8 n <= 23, u16 n <= 575, u32 n <= 8953, u64 n <= 8953,
 *            u128 n <= 8953, big any n
 * Helpers:   big_pow, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/is_prime_small.h"

/* Return the next prime larger than p (p ≥ 2).  */
static idx_t next_prime_small_direct(idx_t p)
{
    for (idx_t q = p + 1; ; ++q) {
        if (is_prime_small(q))
            return q;
    }
}

/* A003415 – arithmetic derivative of n.  */
static seq_t arithmetic_derivative(idx_t n)
{
    if (n < 2)
        return 0;
    seq_t res = 0;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            /* term = n * exp / p  (exact integer) */
            seq_t term = ((seq_t)n * exp) / p;
            res += term;
        }
    }
    if (temp > 1) {               /* remaining large prime factor */
        seq_t term = ((seq_t)n * 1) / temp;
        res += term;
    }
    return res;
}

/* A003961 – prime shift towards larger primes of n.  */
static seq_t prime_shift(idx_t n)
{
    if (n <= 1)
        return 1;
    seq_t prod = 1;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            idx_t nextp = next_prime_small_direct(p);
            prod *= big_pow((seq_t)nextp, (seq_t)exp);
        }
    }
    if (temp > 1) {               /* remaining large prime factor */
        idx_t nextp = next_prime_small_direct(temp);
        prod *= big_pow((seq_t)nextp, (seq_t)1);
    }
    return prod;
}

/* Public sequence function.  */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;
            sum += arithmetic_derivative(d) * prime_shift(e);
            if (d != e)
                sum += arithmetic_derivative(e) * prime_shift(d);
        }
    }
    return sum;
}

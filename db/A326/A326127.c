/*
 * A326127  a(n) = A326126(n) - n, where A326126 gives the sum of all other
 *          divisors of n except the squarefree part of n.
 * https://oeis.org/A326127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = σ(n) - sqfree(n) - n where σ(n) is the sum of all
 *            divisors of n, sqfree(n) is the product of the primes that
 *            appear with an odd exponent in the factorization of n
 * Range:     i8 n <= 89, i16 n <= 9879, i32 n <= 9879, i64 n <= 9879,
 *            i128 n <= 9879, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>


seq_t a(idx_t n)
{
    seq_t sigma = 1;          /* product of σ(p^e) terms */
    seq_t sqfree = 1;         /* product of primes with odd exponent */
    idx_t m = n;

    /* factor 2 separately for speed */
    if ((m & 1) == 0) {
        int exp = 0;
        seq_t p_pow = 1;      /* 2^exp */
        while ((m & 1) == 0) {
            m >>= 1;
            exp++;
            p_pow *= 2;
        }
        /* σ(2^exp) = 2^(exp+1) - 1 */
        sigma *= ((p_pow * 2) - 1);
        if (exp & 1) sqfree *= 2;
    }

    /* odd prime factors */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p != 0) continue;
        int exp = 0;
        seq_t p_pow = 1;      /* p^exp */
        while (m % p == 0) {
            m /= p;
            exp++;
            p_pow *= p;
        }
        /* σ(p^exp) = (p^(exp+1)-1)/(p-1) */
        seq_t term = ((p_pow * p - 1) / (p - 1));
        sigma *= term;
        if (exp & 1) sqfree *= p;
    }

    /* remaining prime factor (exponent 1, odd) */
    if (m > 1) {
        seq_t term = 1 + (seq_t)m;
        sigma *= term;
        sqfree *= m;
    }

    return sigma - sqfree - (seq_t)n;
}

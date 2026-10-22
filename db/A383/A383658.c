/*
 * A383658  Denominator of Dirichlet g.f.: Sum_{n>=1} a(n)/n^s =
 *          zeta(s)^(3/2).
 * https://oeis.org/A383658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, mult
 * Formula:   a(n) = 2^{ sum_{p^e||n} ( e +
 *            floor(e/2)+floor(e/4)+floor(e/8)+... ) }
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where the sum is over the exponents e in the prime factorisation of n. */

#include <stddef.h>

/* compute v2(e!) = number of factors of 2 in e! (Legendre's formula) */
static inline idx_t v2_factorial(idx_t e)
{
    idx_t res = 0;
    for (idx_t q = 2; q <= e; q <<= 1)
        res += e / q;
    return res;
}

/* seq_t is an unsigned integer type determined at compile time */
seq_t a(idx_t n)
{
    if (n == 1)    /* a(1) = 1 */
        return (seq_t)1;

    idx_t x = n;
    idx_t sum_exp = 0;   /* the exponent of 2 in the denominator */

    /* factor 2 separately for speed */
    if ((x & 1) == 0) {
        idx_t e = 0;
        while ((x & 1) == 0) {
            x >>= 1;
            e++;
        }
        sum_exp += e + v2_factorial(e);
    }

    /* trial division for odd primes */
    for (idx_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            idx_t e = 0;
            while (x % p == 0) {
                x /= p;
                e++;
            }
            sum_exp += e + v2_factorial(e);
        }
    }

    /* if there's a remaining prime factor > sqrt(n) */
    if (x > 1) {
        /* e = 1 for that prime */
        sum_exp += 1 + v2_factorial(1);  /* v2_factorial(1) == 0 */
    }

    /* compute 2^{sum_exp} */
    seq_t result = (seq_t)1;
    result <<= (size_t)sum_exp;  /* shift by computed exponent */
    return result;
}

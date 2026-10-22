/*
 * A351751  a(n) = Sum_{p|n, p prime} p^sigma_p(p).
 * https://oeis.org/A351751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n, p prime} p^sigma_p(p)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 2, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*     where sigma_p(p) = 1 + p^p   (divisors of a prime are 1 and p) */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1)           /* a(1) = 0 */
        return (seq_t)0;

    seq_t result = (seq_t)0;
    uint64_t m = (uint64_t)n;   /* integer value of n for factorization */

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            seq_t base = (seq_t)p;
            seq_t pp   = big_pow(base, (seq_t)p);   /* p^p   */
            seq_t exp  = pp + (seq_t)1;             /* 1 + p^p */
            seq_t term = big_pow(base, exp);        /* p^exp */
            result = result + term;
            while (m % p == 0) m /= p;              /* strip all copies of p */
        }
    }

    if (m > 1) {           /* remaining prime factor > sqrt(original n) */
        seq_t base = (seq_t)m;
        seq_t pp   = big_pow(base, (seq_t)m);    /* m^m   */
        seq_t exp  = pp + (seq_t)1;              /* 1 + m^m */
        seq_t term = big_pow(base, exp);         /* m^exp */
        result = result + term;
    }

    return result;
}

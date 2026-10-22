/*
 * A334748  Let p be the smallest odd prime not dividing the squarefree part
 *          of n. Multiply n by p and divide by the product of all smaller odd
 *          primes.
 * https://oeis.org/A334748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * p / ∏_{q<p, odd prime} q where p is the smallest odd
 *            prime not dividing the squarefree part of n
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* Compute the squarefree part of n (product of primes with odd exponent). */
    idx_t t = n;
    seq_t sf = (seq_t)1;
    for (idx_t i = 2; i * i <= t; ++i) {
        if (t % i == 0) {
            int exp = 0;
            while (t % i == 0) {
                t /= i;
                exp++;
            }
            if (exp & 1) {  /* odd exponent */
                sf *= (seq_t)i;
            }
        }
    }
    if (t > 1) {          /* remaining prime factor with exponent 1 */
        sf *= (seq_t)t;
    }

    /* Find smallest odd prime p not dividing sf, and product of all odd
       primes less than p. */
    seq_t prod_smaller = (seq_t)1;   /* product of all odd primes < p */
    for (idx_t q = 3; ; q += 2) {
        if (!is_prime(q))
            continue;
        if (sf % (seq_t)q != 0) {    /* q is p */
            return ((seq_t)n * (seq_t)q) / prod_smaller;
        }
        prod_smaller *= (seq_t)q;    /* include q in the product of smaller odds */
    }
}

/*
 * A285851  Denominator of the ratio of alternate consecutive prime gaps:
 *          Denominator((prime(n + 3) - prime(n + 2))/(prime(n + 1) -
 *          prime(n))).
 * https://oeis.org/A285851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator((prime(n + 3) - prime(n + 2))/(prime(n + 1) -
 *            prime(n)))
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"          /* seq_t gcd(seq_t a, seq_t b) */
#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k) */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;               /* undefined for n < 1 */

    /* Retrieve the first four consecutive primes needed for index n */
    seq_t p_n   = (seq_t)get_prime(n);       /* prime(n)   */
    seq_t p_np1 = (seq_t)get_prime(n + 1);   /* prime(n+1) */
    seq_t p_np2 = (seq_t)get_prime(n + 2);   /* prime(n+2) */
    seq_t p_np3 = (seq_t)get_prime(n + 3);   /* prime(n+3) */

    /* Compute the two prime gaps */
    seq_t g1 = p_np1 - p_n;          /* prime(n+1) - prime(n)   */
    seq_t g2 = p_np3 - p_np2;        /* prime(n+3) - prime(n+2) */

    /* Reduce the fraction g2 / g1 and return its denominator */
    seq_t d = gcd(g1, g2);
    return g1 / d;
}

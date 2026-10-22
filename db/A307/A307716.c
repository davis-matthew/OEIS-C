/*
 * A307716  Denominator of the barycenter of first n primes defined as a(n) =
 *          denominator(Sum_{i=1..n} (i*prime(i)) / Sum_{i=1..n} prime(i)).
 * https://oeis.org/A307716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, look
 * Formula:   a(n) = denominator( Sum_{i=1..n} i*prime(i) / Sum_{i=1..n}
 *            prime(i) )
 * Range:     u8 n <= 7, u16 n <= 41, u32 n <= 5823, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Handle the case n < 1 – the sequence is undefined there, but we return 1 */
    if (n < 1)
        return (seq_t)1;

    seq_t num = 0;   /* Σ i*prime(i)   */
    seq_t den = 0;   /* Σ prime(i)     */

    for (idx_t i = 1; i <= n; ++i) {
        uint32_t p = get_prime(i);              /* p = prime(i)       */
        num += (seq_t)i * (seq_t)p;             /* add i * p          */
        den += (seq_t)p;                        /* add p              */
    }

    /* Reduce the fraction num/den and return the denominator in lowest terms */
    seq_t g = gcd(num, den);
    return den / g;
}

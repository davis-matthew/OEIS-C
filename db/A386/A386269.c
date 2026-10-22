/*
 * A386269  a(n) = denominator((prime(n) - 2)/(n - 1)).
 * https://oeis.org/A386269
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = denominator((prime(n) - 2)/(n - 1))
 * Range:     u8 n <= 55, u16 n <= 8346, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 2. */
    if (n < 2) return 0;

    /* Obtain the n‑th prime.  get_prime returns a 32‑bit value; cast to
       seq_t so that all arithmetic is performed with the target type. */
    seq_t p = (seq_t)get_prime(n);

    /* Compute the numerator and denominator of the fraction (p‑2)/(n‑1). */
    seq_t num = p - (seq_t)2;
    seq_t den = (seq_t)(n - 1);

    /* Reduce the fraction; gcd is defined in the utils library. */
    seq_t g = gcd(num, den);

    return den / g;
}

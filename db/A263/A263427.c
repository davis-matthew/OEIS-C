/*
 * A263427  Denominator of (prime(prime(n))+2)/(prime(n)+2).
 * https://oeis.org/A263427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = denominator of (prime(prime(n))+2)/(prime(n)+2)
 * Range:     u8 n <= 17, u16 n <= 2968, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = (prime(n)+2) / gcd(prime(n)+2, prime(prime(n))+2) */
#include "../utils/gcd.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Retrieve the n-th prime p and the p-th prime q */
    uint32_t p = get_prime(n);          /* p = prime(n)   */
    uint32_t q = get_prime((idx_t)p);  /* q = prime(p)   */

    /* Build numerator and denominator as seq_t */
    seq_t den = (seq_t)p + (seq_t)2;    /* prime(n)+2     */
    seq_t num = (seq_t)q + (seq_t)2;    /* prime(prime(n))+2 */

    /* Simplify the fraction */
    seq_t g = gcd(num, den);
    return den / g;
}

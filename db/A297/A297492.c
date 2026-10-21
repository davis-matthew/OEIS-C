/*
 * A297492  a(n) = (1/2) * Sum_{|k|<=2*sqrt(p)} k^6*H(4*p-k^2) where H() is
 *          the Hurwitz class number and p is the n-th prime.
 * https://oeis.org/A297492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*prime(n)^4 – 9*prime(n)^2 – 5*prime(n) – 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 39, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* first get the n-th prime – it comes as an unsigned 32‑bit int */
    seq_t p = (seq_t)get_prime(n);

    /* compute p^2 and p^4 using only seq_t arithmetic */
    seq_t p2 = p * p;
    seq_t p4 = p2 * p2;

    /* compute the polynomial 5p^4 – 9p^2 – 5p – 1 */
    seq_t val = 5 * p4;
    val -= 9 * p2;
    val -= 5 * p;
    val -= 1;

    return val;
}

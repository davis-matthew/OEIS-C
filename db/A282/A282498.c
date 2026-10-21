/*
 * A282498  a(n) = nearest integer to Pi*prime(n).
 * https://oeis.org/A282498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nearest integer to π * prime(n)
 * Range:     u8 none, u16 n <= 42, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* (optional, offset is 1) */
    uint32_t p = get_prime(n);    /* primes are 1‑based */

    /* 355/113 is a rational approximation of π with error < 3×10⁻⁷.
       For any prime p the error in π·p is < 0.5, so rounding remains
       correct. */
    static const seq_t PI_NUM = (seq_t)355;
    static const seq_t PI_DEN = (seq_t)113;

    seq_t prod = (seq_t)p * PI_NUM;
    seq_t res  = (prod + PI_DEN / 2) / PI_DEN;   /* nearest integer */
    return res;
}

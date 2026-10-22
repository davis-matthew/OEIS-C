/*
 * A384516  Consecutive states of the linear congruential pseudo-random number
 *          generator 410092949*s mod 2^32 when started at s=1.
 * https://oeis.org/A384516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 410092949 * a(n-1) mod 2^32, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the n-th term of OEIS sequence A384516.
 * a(1) = 1
 * a(n) = (410092949 * a(n-1)) mod 2^32   for n >= 2
 *
 * Parameters:
 *   n - the index (1-based), type idx_t (unsigned 64‑bit)
 *
 * Returns:
 *   a(n) cast to seq_t; seq_t may be 8, 16, 32, 64, 128 bits or an arbitrary‑precision integer.
 */
seq_t a(idx_t n)
{
    /* For n==1, the term is simply 1. */
    if (n == 1) return (seq_t)1;

    /* Fast exponentiation: a(n) = 410092949^(n-1) mod 2^32. */
    uint64_t e = n - 1;          /* exponent (unsigned) */
    uint32_t base = 410092949u;  /* multiplier */
    uint32_t res  = 1u;          /* identity element */

    while (e) {
        if (e & 1u)
            res = (uint32_t)((uint64_t)res * base & 0xFFFFFFFFu);
        base = (uint32_t)((uint64_t)base * base & 0xFFFFFFFFu);
        e >>= 1;
    }

    return (seq_t)res;
}

/*
 * A323704  Number of cubic residues (including 0) modulo the n-th prime.
 * https://oeis.org/A323704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p-1)/3 + 1 if (p-1) mod 3 = 0, a(n) = p otherwise where
 *            p = the n-th prime (with p(1)=2)
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)            /* undefined for n<1, return 0 safely */
        return 0;

    /* obtain the n-th prime (index 1‑based) */
    seq_t p = (seq_t)get_prime(n);

    /* number of cubic residues modulo p, including 0 */
    if ((p - 1) % 3 == 0)
        return (p - 1) / 3 + 1;
    else
        return p;
}

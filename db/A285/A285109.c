/*
 * A285109  a(n) = n multiplied by its smallest prime factor; a(1) = 1.
 * https://oeis.org/A285109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * smallest_prime_factor(n) (with a(1)=1)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/smallest_prime_factor.h"

/* Return the n-th term of A285109.
   n is 1-based; if n<2 the value is defined as 1. */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)1;

    /* Use the helper that returns the smallest prime factor of a
       64‑bit unsigned integer.  The helper is defined in
       utils/smallest_prime_factor.h. */
    uint64_t spf = smallest_prime_factor((uint64_t)n);

    return (seq_t)n * (seq_t)spf;
}

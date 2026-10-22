/*
 * A372710  a(n) = Sum_{k=1..n} sigma(n*k).
 * https://oeis.org/A372710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A372710
   Formula: a(n) = Sum_{k=1..n} sigma(n*k)   (sigma = sum‑of‑divisors) */

#include <stdint.h>
#include <stddef.h>
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t sum = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k)
    {
        /* Compute sigma(n*k) using the provided helper */
        sum += sigma_of(n * k);
    }
    return sum;
}

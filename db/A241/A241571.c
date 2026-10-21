/*
 * A241571  Numbers n such that 2*n+15 is not a prime.
 * https://oeis.org/A241571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 162, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A241571: numbers n such that 2*n+15 is not prime.
   a(1) = 0, a(2) = 3, a(3) = 5, … */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* The function is defined for n >= 1 only. */
    if (n == 0)
        return (seq_t)0;  /* guard against underflow */

    idx_t count = 0;
    idx_t cand   = 0;

    for (;;)
    {
        /* Compute 2*cand + 15 in seq_t form. */
        seq_t val = (seq_t)cand * (seq_t)2 + (seq_t)15;

        /* If the value is not prime, it is part of the sequence. */
        if (!is_prime_big(val))
        {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
        ++cand;
    }
}

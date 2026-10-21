/*
 * A256558  Number of ways to write n = p + floor(k*(k+1)/4), where p is a
 *          prime and k is a positive integer.
 * https://oeis.org/A256558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2026, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count representations n = p + floor(k*(k+1)/4) with p prime. */
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* For n < 2 there cannot be a prime p ≥ 2, so the count is zero. */
    if (n < 2)
        return 0;

    seq_t count = 0;

    for (idx_t k = 1;; ++k)
    {
        /* Compute floor(k*(k+1)/4) as an integer division. */
        seq_t part = (seq_t)k * ((seq_t)k + 1) / 4;

        /* Break if the remaining part would force a prime < 2. */
        if (part > (seq_t)n - 2)
            break;

        seq_t p = (seq_t)n - part;
        if (p >= 2 && is_prime((idx_t)p))
            ++count;
    }

    return count;
}

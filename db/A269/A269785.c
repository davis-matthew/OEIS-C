/*
 * A269785  Primes p such that 2*p + 23 is a square.
 * https://oeis.org/A269785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A269785: primes p such that 2*p + 23 is a square.
   Equivalent formulation: p = 2*k^2 + 2*k - 11  for integers k ≥ 3. */

/* The implementation performs a linear search over k,
   testing each value of p for primality using the provided
   is_prime_big function. The result is the n-th such prime. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for 0, return 0 for safety   */

    idx_t count = 0;                /* number of primes found so far          */
    seq_t k = 3;                    /* start of the k‑parameter (k ≥ 3)      */

    for (;; ++k)
    {
        seq_t p = 2 * k * k + 2 * k - (seq_t)11;   /* p = 2k² + 2k – 11           */

        if (is_prime_big(p))
        {
            ++count;
            if (count == n)
                return p;          /* return the n‑th prime found          */
        }
    }
}

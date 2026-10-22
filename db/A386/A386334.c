/*
 * A386334  Primes without {1, 8} as digits.
 * https://oeis.org/A386334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 2134, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime that contains no digits 1 or 8
   (OEIS A386334). By brute‑force search using is_prime()
   and digit filtering. */
#include "../utils/is_prime.h"
#include <stdbool.h>
#include <stdint.h>

/* External linkage as requested */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; unused below 1 */

    idx_t count = 0;
    seq_t cand   = 2;              /* start from the first prime */

    while (1)
    {
        /* Test primality (cand fits in idx_t for the needed range) */
        if (is_prime((idx_t)cand))
        {
            /* Reject numbers containing digit 1 or 8 */
            uint64_t tmp = (uint64_t)cand;
            bool bad = false;
            while (tmp > 0)
            {
                uint8_t d = (uint8_t)(tmp % 10);
                if (d == 1 || d == 8) { bad = true; break; }
                tmp /= 10;
            }
            if (!bad)
            {
                ++count;
                if (count == n) return cand;
            }
        }

        /* Advance to next candidate (skip evens after 2) */
        if (cand == 2) cand = 3;
        else           cand += 2;
    }
}

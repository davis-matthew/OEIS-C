/*
 * A358790  a(n) is the least prime p such that (2*n+1)^2 + p^2 is twice a
 *          prime.
 * https://oeis.org/A358790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime p such that ((2*n+1)^2 + p^2)/2 is prime
 * Range:     u8 n <= 6, u16 n <= 568, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    // Compute (2*n+1)^2 in seq_t.
    seq_t nn   = (seq_t)n;
    seq_t val  = 2 * nn + 1;        // 2*n+1
    seq_t base = val * val;         // (2*n+1)^2

    // Search for the smallest prime p>2 such that ((2*n+1)^2 + p^2)/2 is prime.
    uint64_t p = 3;                 // first odd prime

    for (;;)
    {
        seq_t pp  = (seq_t)p * (seq_t)p;      // p^2
        seq_t sum = base + pp;                // (2*n+1)^2 + p^2
        seq_t q   = sum / 2;                  // (sum)/2, always integer for odd p

        if (isPrimeGeneric(q))
            return (seq_t)p;                  // found smallest prime p

        p = next_prime(p);                    // next prime after current p
    }
}

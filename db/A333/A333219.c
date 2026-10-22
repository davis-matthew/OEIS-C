/*
 * A333219  Heinz number of the n-th composition in standard order.
 * https://oeis.org/A333219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the Heinz number of the n‑th composition in standard
 *            order. The composition is obtained from n‑1 by taking the
 *            positions of 1’s in the reversed binary expansion (positions
 *            counted from 1), prepending 0, computing successive differences,
 *            reversing, and taking the product of primes indexed...
 * Range:     u8 n <= 255, u16 n <= 9504, u32 n <= 9504, u64 n <= 9504,
 *            u128 n <= 9504, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            // defined for n >= 1, but return 0 for safety
    idx_t k = n - 1;                 // composition index
    seq_t prod = 1;                  // product of primes
    idx_t cur = 0;                   // previous position (starts with 0)
    idx_t bit = 0;                   // bit index for traversing k

    while (k != 0) {
        if (k & 1U) {                // bit set at position bit (0‑based)
            idx_t pos = bit + 1;     // positions are 1‑based
            idx_t diff = pos - cur; // first difference
            prod *= (seq_t)get_prime(diff);
            cur = pos;
        }
        k >>= 1;
        ++bit;
    }
    /* for k == 0 (n == 1) the loop is skipped and prod stays 1, which is
       the correct value for the empty composition. */
    return prod;
}

/*
 * A320044  Number of positive integers k < prime(n)/2 with {k^3/prime(n)} >
 *          1/2, where {x} = x - floor(x) is the fractional part of a real
 *          number x.
 * https://oeis.org/A320044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 6558, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_for_prime, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A320044:  a(n) = number of k < prime(n)/2
   for which the fractional part of k³ / prime(n) exceeds 1/2. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/count_for_prime.h"

/* Public entry point for the sequence.  n is 1‑based (offset = 1). */
seq_t a(idx_t n)
{
    if (n == 0u)
        return 0;                          /* undefined for n=0, stay safe */

    uint32_t p = get_prime(n);               /* prime(n)   */
    return count_for_prime(p);
}

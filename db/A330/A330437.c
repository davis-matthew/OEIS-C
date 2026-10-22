/*
 * A330437  Length of trajectory of n under the map n -> n - 1 + n/gpf(n) or 0
 *          if no fixed point is reached, where gpf(n) is the greatest prime
 *          factor of n.
 * https://oeis.org/A330437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the trajectory of n under n -> n-1 + n/gpf(n)
 *            (0 if no prime is reached). The function stops when a prime or 1
 *            is encountered because such numbers are fixed points of the map
 * Range:     u8 n <= 789, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gpf.h"    /* uint64_t gpf(uint64_t x);            */
#include "../utils/is_prime.h" /* bool is_prime(idx_t x);  */

/* External linkage: computes the desired sequence term. */
seq_t a(idx_t n)
{
    /* Start of trajectory */
    seq_t cur     = (seq_t)n;
    seq_t length  = 1;

    /* Prime or 1 are fixed points – terminate immediately. */
    if (n == 1 || is_prime(n))
        return 1;

    /* Iterate until a fixed point (prime or 1) is found. */
    while (1)
    {
        /* gpf expects a 64‑bit unsigned integer.  cur fits in it
           for all n up to the tested limits. */
        uint64_t cur64 = (uint64_t)cur;
        uint64_t g     = gpf(cur64);          /* greatest prime factor */

        /* Compute the next term: cur - 1 + cur/g. */
        seq_t next = cur - 1 + cur / (seq_t)g;
        ++length;

        /* Check for a fixed point. */
        if (next == 1 || is_prime((idx_t)next))
            return length;

        cur = next;
    }

    /* If control ever reaches here, no fixed point was found. */
    return 0;
}

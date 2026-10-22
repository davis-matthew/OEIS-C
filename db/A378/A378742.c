/*
 * A378742  Primitively abundant numbers k for which A378664(k) = 6, where
 *          A378664 is the greatest divisor d of n such that sigma(d) <= 2*d <
 *          A003961(d), or 1 if no such divisor exists.
 * https://oeis.org/A378742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1321, u32 n <= 9933, u64 n <= 9933,
 *            u128 n <= 9933, big any n
 * Helpers:   excluded
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A378742
 * a(n) = 6 * p_n
 * where p_n is the n‑th prime that is NOT 3,5,7,13,19,23.
 * (OEIS offset: a(1)=12, a(2)=66, …)
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/excluded.h"

/* primality test using 6k±1 wheel – safe for 64‑bit integers */
static bool is_prime(uint64_t x)
{
    if (x < 2)                 return false;
    if (x == 2 || x == 3)      return true;
    if (x % 2 == 0 || x % 3 == 0) return false;
    for (uint64_t i = 5; i <= x / i; i += 6)
    {
        if (x % i == 0 || x % (i + 2) == 0) return false;
    }
    return true;
}

/* a(n) for all n >= 1, with idx_t (uint64_t) indexing */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* undefined but safe */

    uint64_t count = 0;
    uint64_t candidate = 1;    /* start search from 2 */

    while (count < n)
    {
        do { candidate++; } while (!is_prime(candidate));
        if (!excluded(candidate))
            count++;
    }

    return (seq_t)6 * (seq_t)candidate;
}

/*
 * A249061  a(n) is the least number of successive numbers 1, 2, 3, ... which
 *          when added to n produce a prime number.
 * https://oeis.org/A249061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least k ≥ 1 such that n + k(k+1)/2 is prime
 * Range:     u8 n <= 279, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Compute the A249061 term for index n. */
seq_t a(idx_t n)
{
    seq_t start = (seq_t)n;   /* base value n in seq_t            */
    seq_t k      = 1;         /* current trial k                  */
    seq_t tri    = 1;         /* triangular number k(k+1)/2 for k=1 */
    seq_t val    = start + tri;  /* current candidate sum            */

    while (!is_prime_big(val))
    {
        k += 1;               /* next k */
        tri += k;             /* new triangular number */
        val = start + tri;    /* candidate sum for new k */
    }
    return k;                 /* smallest k giving a prime      */
}

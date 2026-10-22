/*
 * A340420  The number of steps that n requires to reach 1 under the map: m ->
 *          m/2 if m is even, m-> 3*m + 1 if m is an odd prime, otherwise m ->
 *          m - 1. a(n) = -1 if 1 is never reached.
 * https://oeis.org/A340420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 8415, u32 n <= 8415, u64 n <= 8415,
 *            u128 n <= 8415, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Map based computation: a(n) = number of steps to reach 1 under
   m -> m/2 if even, m -> 3*m + 1 if odd prime, m -> m - 1 otherwise.
   Returns -1 if 1 is never reached (conjectured never). */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Safety: offset starts at 1, but guard against n==0. */
    if (n == 0) return (seq_t)0;

    seq_t steps = (seq_t)0;
    seq_t m = (seq_t)n;

    if (m == (seq_t)1) return steps;

    while (m != (seq_t)1)
    {
        /* Even number: divide by 2 */
        if ((m & (seq_t)1) == 0)
        {
            m = m >> 1;
        }
        else if (is_prime_big(m))
        {
            /* Odd prime */
            m = m * (seq_t)3 + (seq_t)1;
        }
        else
        {
            /* Odd composite (or 1, but we stop before 1) */
            m = m - (seq_t)1;
        }
        steps++;
    }
    return steps;
}

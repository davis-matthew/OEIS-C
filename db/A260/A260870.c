/*
 * A260870  Least k>0 such that k^2 + (2n+1-k)^2 is prime, or 0 if no such k
 *          exists.
 * https://oeis.org/A260870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k > 0 such that k^2 + (2n+1-k)^2 is prime. Returns
 *            0 if no such k exists
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* N = 2n+1 (the odd number to be split) */
    idx_t N = (idx_t)2 * n + (idx_t)1;

    /* k runs from 1 to N-1.  For each, test whether k^2 + (N-k)^2 is prime. */
    for (idx_t k = (idx_t)1; k < N; ++k)
    {
        idx_t kk      = k * k;
        idx_t rest    = (N - k) * (N - k);
        idx_t sum     = kk + rest;
        if (is_prime(sum))
            return (seq_t)k;          /* found the minimal k */
    }
    return (seq_t)0;                  /* no valid k found */
}

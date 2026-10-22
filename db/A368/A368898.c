/*
 * A368898  a(n) = Sum_{k=0..floor(n/4)} n^k * binomial(n-3*k,k).
 * https://oeis.org/A368898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} n^k * binomial(n-3*k, k) * (OEIS
 *            A368898)
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 24, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Maximum k is floor(n/4). */
    idx_t kmax = n >> 2;          /* division by 4 is safe because n is unsigned. */
    seq_t result = 0;
    seq_t power  = 1;              /* n^k, starting with k=0 */

    for (idx_t k = 0; k <= kmax; ++k)
    {
        /* binomial(n-3*k, k) */
        idx_t nn = n - 3 * k;
        seq_t b = binomial(nn, k);

        /* term = power * b = n^k * binomial(n-3*k, k) */
        result += power * b;

        /* Prepare power for next k: multiply by n */
        power *= (seq_t)n;
    }

    return result;
}

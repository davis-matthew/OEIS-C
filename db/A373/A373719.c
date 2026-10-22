/*
 * A373719  a(n) = Sum_{k=0..floor(n^2/(2*n+1))} binomial(n * (n-2*k),k).
 * https://oeis.org/A373719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n^2/(2*n+1))} binomial(n*(n-2*k),k)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* upper bound for k */
    idx_t kmax = (n * n) / (2 * n + 1);

    seq_t sum = 0;
    for (idx_t k = 0; k <= kmax; ++k)
    {
        idx_t N = n * (n - 2 * k);
        sum += binom(N, k);
    }
    return sum;
}

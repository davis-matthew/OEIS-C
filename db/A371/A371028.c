/*
 * A371028  a(n) = Sum_{k=1..n} (n + k - 1)! / (k!*(n - k)!) if n > 0 and a(0)
 *          = 1. Row sums of A370983.
 * https://oeis.org/A371028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (n + k - 1)! / (k! * (n - k)!) (a(0) = 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   term_next
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/term_next.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    seq_t term = (seq_t)n;            /* term for k = 1 */
    sum += term;

    for (idx_t k = 1; k < n; ++k)
    {
        term = term_next(term, n, k);
        sum += term;
    }

    return sum;
}

/*
 * A360152  a(n) = Sum_{k=0..floor(n/3)} binomial(2*n-5*k,n-3*k).
 * https://oeis.org/A360152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 25 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A360152
   Formula: a(n) = Sum_{k=0..floor(n/3)} binomial(2*n-5*k, n-3*k)
   Offset: 0  (a(0) = 1)
*/

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;
    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t part1 = 2 * n - 5 * k;
        idx_t part2 = n - 3 * k;
        sum += binomial(part1, part2);
    }
    return sum;
}

/*
 * A245547  Write n>=1 as either n=2^k-2^r with 0 <= r <= k-1, in which case
 *          a(2^k-2^r)=(k-r)*(k-r+1)/2, or as n=2^k-2^r+j with 2 <= r <= k-1,
 *          1 <= j < 2^r-1, in which case a(2^k-2^r+j)=(k-r)*(k-r+1)*a(j).
 * https://oeis.org/A245547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   calc_a
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A245547:  a(1) = 1, 
 * a(n) = (k-r)*(k-r+1)/2                     if n = 2^k - 2^r           (0 ≤ r ≤ k-1)
 * a(n) = (k-r)*(k-r+1) * a(j)                 if n = 2^k - 2^r + j
 *                               (2 ≤ r ≤ k-1, 1 ≤ j < 2^r-1)
 * Here k is the unique integer with 2^(k-1) ≤ n < 2^k.
 */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/calc_a.h"

seq_t a(idx_t n)
{
    return calc_a(n);
}

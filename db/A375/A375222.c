/*
 * A375222  a(n) is the number of permutations of the multiset 1,1, 2,2, ...,
 *          n,n such that exactly one pair k,k stays at its initial locations
 *          2k-1, 2k.
 * https://oeis.org/A375222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * A374980(n-1) where A374980(m) is the number of
 *            permutations of the multiset 1,1,2,2,…,m,m that have no pair in
 *            its original position
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 19,
 *            big any n
 * Helpers:   A374980
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/A374980.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1; undefined for n=0 */
    idx_t m = n - 1;                   /* need A374980(m) */
    seq_t val = A374980(m);
    return ((seq_t)n) * val;
}

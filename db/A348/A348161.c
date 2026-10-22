/*
 * A348161  Number of factorizations of (n,n) into one or two pairs (i,j) with
 *          i > 0, j > 0 (and if i=1 then j=1).
 * https://oeis.org/A348161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil((tau(n)-2)*tau(n)/2) + 1 (tau(n) = number of
 *            divisors of n)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1, behaviour below offset is irrelevant */
        return 0;

    seq_t tau = divisor_count_u64(n);           /* tau(n) */
    if (tau <= 2)
        return 1;                               /* handles n=1 and n with exactly two divisors */

    seq_t prod = (tau - 2) * tau;               /* (tau-2)*tau >= 0 */
    seq_t half = (prod + 1) / 2;                /* ceil division: ceil(prod/2) = (prod+1)/2 */
    return half + 1;                            /* final result */
}

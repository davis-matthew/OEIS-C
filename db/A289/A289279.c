/*
 * A289279  Number of odd composite numbers in ]2*(n-1)^2, 2*n^2[.
 * https://oeis.org/A289279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of odd composite integers in the open interval
   ( 2*(n-1)^2 , 2*n^2 ).  For each odd candidate x in that range,
   the function tests for primality using the provided is_prime_small
   routine.  Only values that are odd, greater than 1, and not
   prime are counted. */
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n=0, but return 0 */
        return 0;

    /* Compute bounds as seq_t to avoid overflow. */
    seq_t nm1 = (seq_t)(n - 1);
    seq_t lower = (seq_t)2 * (nm1 * nm1);          /* 2*(n-1)^2 */
    seq_t upper = (seq_t)2 * ((seq_t)n * n);       /* 2*n^2 */

    /* First odd integer strictly greater than lower. */
    seq_t start = lower + 1;
    if ((start & 1) == 0)
        start += 1;

    /* Last odd integer strictly less than upper. */
    seq_t end = upper - 1;
    if ((end & 1) == 0)
        end -= 1;

    seq_t count = 0;
    for (seq_t x = start; x <= end; x += 2)
    {
        /* x is odd by construction; skip 1 if it appears. */
        if (x <= 1)
            continue;

        /* Count if odd composite. */
        if (!is_prime_small(x))
            count += 1;
    }
    return count;
}

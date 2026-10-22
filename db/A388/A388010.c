/*
 * A388010  Irregular table T(n, k), n > 0, k = 1..A246600(n), read by rows;
 *          the n-th row lists the divisors d of n such that the binary
 *          representation of d can be obtained from the binary representation
 *          of n by changing any number of 1's to 0's.
 * https://oeis.org/A388010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   not stated
 * Range:     u8 n <= 627, u16 n <= 9888, u32 n <= 9888, u64 n <= 9888,
 *            u128 n <= 9888, big any n
 * Helpers:   int_sqrt64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation generates the n‑th entry of OEIS sequence A388010.
   It enumerates the rows of the irregular divisor table row by row,
   keeping a cumulative counter.  For each integer m it collects all
   divisors d such that d & ~m == 0 (i.e. the binary bits of d are a
   subset of the bits of m).  The divisors are output in ascending order
   for each row – small divisors are processed immediately, larger
   divisors are stored temporarily and emitted in reverse order.  The
   function returns the value of the sequence as a seq_t. */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/int_sqrt64.h"

seq_t a(idx_t N)
{
    idx_t count = 0;                     /* how many terms have been output so far */
    for (idx_t num = 1; ; ++num)         /* iterate over rows (n = 1, 2, …) */
    {
        idx_t limit = int_sqrt64(num);
        size_t cap = (size_t)limit + 1;
        idx_t *large = (idx_t *)malloc(cap * sizeof(idx_t));
        if (!large) return (seq_t)0;     /* allocation failure – should never happen */
        size_t large_len = 0;

        for (idx_t d = 1; d <= limit; ++d)
        {
            if (num % d == 0)
            {
                idx_t d2 = num / d;

                /* small divisor d – already in ascending order */
                if ((d & ~num) == 0)
                {
                    ++count;
                    if (count == N)
                    {
                        free(large);
                        return (seq_t)d;
                    }
                }

                /* large divisor d2 – store for later (reverse order) */
                if (d2 != d && ((d2 & ~num) == 0))
                {
                    large[large_len++] = d2;
                }
            }
        }

        /* output large divisors in ascending order (reverse of collection) */
        for (size_t i = large_len; i > 0; --i)
        {
            ++count;
            if (count == N)
            {
                seq_t res = (seq_t)large[i - 1];
                free(large);
                return res;
            }
        }
        free(large);
    }
}

/*
 * A359161  Numbers of the form 4u+1 with an even number of prime factors
 *          (counted with multiplicity).
 * https://oeis.org/A359161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth number of the form 4u+1 that has an even number
 *            of prime * factors (counted with multiplicity). This follows
 *            OEIS A359161
 * Range:     u8 n <= 33, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    idx_t count   = 0;
    idx_t cand    = 1;              /* the sequence of numbers 4u+1, u = 0,1,2,... */
    while (1)
    {
        seq_t omega = bigomega(cand);  /* number of prime factors with multiplicity */
        if ((omega & (seq_t)1) == 0)   /* even number of prime factors? */
        {
            count++;
            if (count == n)
                return (seq_t)cand;     /* convert to seq_t on return */
        }
        cand += 4;        /* next number of the form 4u+1 */
    }
}

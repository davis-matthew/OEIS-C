/*
 * A269871  Indices of square pyramidal numbers (A000330) that are the sum of
 *          4 but no fewer nonzero squares.
 * https://oeis.org/A269871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   legendre_form
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes the n-th term of OEIS sequence A269871.
   A number n is included iff the square pyramidal number
   S(n) = n*(n+1)*(2n+1)/6 is of the form 4^a(8b+7) (Legendre's theorem),
   i.e. it cannot be expressed as a sum of three squares.
   The n-th such index is returned.
*/

#include <stdbool.h>
#include "../utils/legendre_form.h"

/* Sequence generator */
seq_t a(idx_t n)
{
    idx_t count = 0;    /* how many qualifying indices have been found   */
    idx_t m     = 1;    /* current index to test                          */

    while (count < n)
    {
        /* Compute S(m) = m*(m+1)*(2m+1)/6 safely in seq_t */
        seq_t a_m = (seq_t)m;                     /* cast to seq_t early       */
        seq_t b   = a_m + (seq_t)1;               /* (m+1)                     */
        seq_t c   = (seq_t)2 * a_m + (seq_t)1;    /* (2m+1)                    */

        seq_t S = a_m * b;                        /* m*(m+1)                   */
        S = S * c;                                /* m*(m+1)*(2m+1)            */
        S = S / (seq_t)6;                         /* divide by 6                */

        if (legendre_form(S))
        {
            count++;
            if (count == n)
                return (seq_t)m;                  /* return the nth qualifying index */
        }

        m++;                                      /* next integer to test          */
    }

    return (seq_t)0;   /* unreachable, keeps compiler happy */
}

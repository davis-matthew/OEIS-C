/*
 * A277215  a(n) is the smallest even number not congruent to 1 modulo 3 that
 *          starts a (2n+1)-element alternating sequence of x/2 and (3x+1)
 *          iterations ending in the maximum of its Collatz trajectory.
 * https://oeis.org/A277215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements OEIS A277215.
   For n > 0 the function finds the smallest odd multiplier q
   (q not divisible by 3) such that
       a = 2*(q*2^n - 1)
   is the start of a 2n+1 element alternating Collatz sequence
   ending at v = 2*(q*3^n - 1), and this v is the maximum of the
   Collatz trajectory of a (before the trajectory reaches 1).
   The algorithm iterates q, simulates the Collatz trajectory from a
   until 1 is reached or a value > v is encountered.  If the
   trajectory maximum equals v, that q is minimal and a is returned.
   For n = 0 the sequence value is 0.  */

#include <stdbool.h>
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* a(0) is defined to be 0 */
        return (seq_t)0;

    /* Precompute 3^n once, it is used for every candidate q. */
    seq_t pow3_n = pow3(n);

    for (idx_t q = 1;; q += 2)          /* iterate over odd q only */
    {
        if (q % 3 == 0)
            continue;                  /* skip multiples of 3 */

        /* a = 2*(q*2^n - 1)  */
        seq_t a_val = ((seq_t)q << n); // q * 2^n
        a_val -= (seq_t)1;
        a_val <<= 1;                   // *2

        /* v = 2*(q*3^n - 1)  */
        seq_t v_val = ((seq_t)q * pow3_n);
        v_val -= (seq_t)1;
        v_val <<= 1;                   // *2

        /* Simulate Collatz starting from a_val until 1 or
           until a value greater than v_val is produced.          */
        seq_t cur   = a_val;
        seq_t maxv  = a_val;
        bool exceeded = false;

        while (cur != (seq_t)1)
        {
            if ((cur & (seq_t)1) == 0)      /* even */
                cur >>= 1;                  /* /2  */
            else                            /* odd  */
                cur = cur * (seq_t)3 + (seq_t)1;

            if (cur > v_val)                /* exceeded v_val */
            {
                exceeded = true;
                break;
            }
            if (cur > maxv)
                maxv = cur;
        }

        if (!exceeded && maxv == v_val)
            return a_val;   /* minimal q found */
    }
}

/*
 * A364688  Number of 8-cycles in the hypercube graph Q_n.
 * https://oeis.org/A364688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-4)*n*(n-1)*(n-2)*(27*n-79) (for all n>=0)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Common factors */
    seq_t sn      = (seq_t)n;
    seq_t term    = sn * (sn - 1) * (sn - 2);
    seq_t factor  = (seq_t)27 * sn - (seq_t)79;   /* 27*n-79 */
    term *= factor;

    if (n < 4)
    {
        /* Right shift by (4-n).  For n=0..3 the shift is at most 4. */
        idx_t shift = 4 - n;
        return term >> shift;
    }
    else
    {
        idx_t shift = n - 4;

        /* Handle shift for 64‑bit types to avoid undefined behaviour. */
        if (sizeof(seq_t) <= 8)
        {
            seq_t res = term;
            while (shift--)
            {
                /* Detect overflow before shifting left by 1. */
                if (res > (UINT64_MAX >> 1))
                    return 0;           /* value exceeds 64‑bit range */
                res <<= 1;
            }
            return res;
        }
        else
        {
            seq_t res = term;
            while (shift--)
                res <<= 1;
            return res;
        }
    }
}

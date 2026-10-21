/*
 * A297646  Heptagonal numbers (A000566) in which parity of digits alternates.
 * https://oeis.org/A297646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 19, u32 n <= 196, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   alternates_parity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A297646: Heptagonal numbers with alternating digit parity.
 * Formula (search form):  
 *   a(n) = the n-th number of the form H_i = i*(5*i-3)/2  
 *         for which the decimal digits alternate in parity.
 */

#include <stdbool.h>
#include "../utils/alternates_parity.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* undefined for n<1, return 0 as sentinel */

    idx_t count = 0;

    for (idx_t i = 1; ; ++i)
    {
        seq_t i_seq = (seq_t)i;
        /* Heptagonal number: H_i = i*(5*i-3)/2 */
        seq_t h = i_seq * ((i_seq * 5) - 3) / 2;

        if (alternates_parity(h))
        {
            ++count;
            if (count == n)
                return h;
        }
    }
}

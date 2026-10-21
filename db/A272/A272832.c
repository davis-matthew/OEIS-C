/*
 * A272832  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 694", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A272832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2*4^n - 16*3^(n-2) + 2^(n-1) + 1 (for n > 2)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow2, pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    switch (n)
    {
        case 0: return (seq_t)1;
        case 1: return (seq_t)5;
        case 2: return (seq_t)21;
        default:
        {
            seq_t term = (seq_t)2 * pow4(n);
            term -= (seq_t)16 * pow3(n - 2);
            term += pow2(n - 1);
            term += (seq_t)1;
            return term;
        }
    }
}

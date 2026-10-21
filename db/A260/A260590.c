/*
 * A260590  Dropping time of 2*n+1 under the modified Collatz map (A014682).
 * https://oeis.org/A260590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 4108, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  Drop time of 2*n+1 under the modified Collatz mapping:
 *  a(n) = the number of iterations of the mapping 
 *  (x -> (3x+1)/2 if x is odd, else x/2) applied to 2*n+1 
 *  until the value becomes strictly less than 2*n+1.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* 2*n+1 is always odd */
    seq_t start = (seq_t)2 * (seq_t)n + 1;
    seq_t orig  = start;
    seq_t steps = 0;

    while (start >= orig) {
        if ((start & 1) == 0) {        /* even */
            start >>= 1;              /* x/2 */
        } else {                       /* odd */
            start = (start * 3 + 1) >> 1;  /* (3x+1)/2 */
        }
        ++steps;
    }
    return steps;
}

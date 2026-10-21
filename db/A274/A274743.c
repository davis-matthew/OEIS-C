/*
 * A274743  Repunits with odd indices multiplied by 99, i.e., 99*(1, 111,
 *          11111, 1111111, ...).
 * https://oeis.org/A274743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11 * (10^(2*n-1) - 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Index n starts at 1 – a(1) = 99, a(2) = 10989, …) */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* offset-1 guard; not part of the sequence */
        return 0;

    seq_t p = pow10(2 * n - 1); /* 10^(2*n-1) */
    p -= 1;                     /* minus 1 */
    p *= 11;                    /* times 11 */
    return p;
}

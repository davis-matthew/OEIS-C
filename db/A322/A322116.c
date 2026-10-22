/*
 * A322116  Main diagonal of triangle A321600; a(n) = A321600(n,n-1) for n >=
 *          1.
 * https://oeis.org/A322116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - 2 - (-1)^n
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* n is assumed ≥ 1 by the caller */
    seq_t p = pow3(n);          /* 3^n */
    seq_t res = p - 2;          /* subtract 2 */

    /* subtract (-1)^n
       (-1)^n =  1 if n even, -1 if n odd
       so we subtract 1 for even n, subtract -1 (i.e. add 1) for odd n */
    if ((n & 1) == 0)
        res -= 1;              /* n even */
    else
        res += 1;              /* n odd */

    return res;
}

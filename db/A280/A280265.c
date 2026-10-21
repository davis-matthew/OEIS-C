/*
 * A280265  Array with five columns read by rows: H_k(n,2), with rows n >= 0
 *          and columns 0 <= k <= 4, where H_n is the n-th hyperoperation.
 * https://oeis.org/A280265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 38, u32 n <= 53, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is the (n‑th) entry of the array H_k(n,2) read row‑wise.
    Let r = n / 5  and  c = n % 5. Then
        a(n) = H_c(r,2)   (0 ≤ c ≤ 4)
    where the hyperoperations are defined by
        H_0(x,y) = y + 1
        H_1(x,y) = x + y
        H_2(x,y) = x * y
        H_3(x,y) = x^y  (with the special convention 0^y = 1 for y > 0)
        H_4(x,y) = x^^y (tetration; for y = 2 this is x^x)
*/

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    idx_t r = n / 5;          /* row index */
    idx_t c = n % 5;          /* column index */

    seq_t value = 0;

    /* To avoid overflow issues when seq_t is small, convert r only
       when needed and use seq_t arithmetic for operations that
       involve r.  For the large exponentiation (H_4) the exponent
       may exceed the width of seq_t; we cast to seq_t for the
       exponent, which is safe for the supported seq_t sizes. */

    switch (c) {
        case 0:                     /* H_0(r,2) = 2 + 1 = 3 */
            value = (seq_t)3;
            break;
        case 1:                     /* H_1(r,2) = r + 2 */
            value = (seq_t)r + (seq_t)2;
            break;
        case 2:                     /* H_2(r,2) = 2 * r */
            value = (seq_t)r * (seq_t)2;
            break;
        case 3:                     /* H_3(r,2) = r^2  (0^y = 1 for y>0) */
            if (r == 0) {
                value = (seq_t)1;   /* 0^2 = 1 by convention */
            } else {
                value = (seq_t)r * (seq_t)r;
            }
            break;
        case 4:                     /* H_4(r,2) = r^^2 = r^r */
            value = big_pow((seq_t)r, (seq_t)r);
            break;
    }

    return value;
}

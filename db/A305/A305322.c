/*
 * A305322  Repdigit numbers that are divisible by 3.
 * https://oeis.org/A305322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   * a(1) = 0. * For n >= 2 let m = n-1. * Let block = (m-1)/15,
 *            offset = m - 15*block (1 ≤ offset ≤ 15). * Then: * if offset ≤ 3
 *            : length = 3*block + 1, * digit = 3*offset (3, 6, 9) * else if
 *            offset ≤ 6 : length = 3*block + 2, * digit = 3*(offset-3) (3, 6,
 *            9) * else : length = 3*block + 3, * digit...
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 46, u64 n <= 94, u128 n <= 189,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;

    idx_t m = n - 1;                     /* skip the initial zero       */
    idx_t block   = (m - 1) / 15;         /* complete blocks of 15 terms*/
    idx_t offset  = m - 15 * block;      /* 1 … 15 within the block     */

    idx_t  length;
    seq_t  digit;

    if (offset <= 3) {
        length = 3 * block + 1;
        digit  = (seq_t)(3 * offset);    /* 3, 6, 9                     */
    } else if (offset <= 6) {
        length = 3 * block + 2;
        digit  = (seq_t)(3 * (offset - 3)); /* 3, 6, 9                  */
    } else {
        length = 3 * block + 3;
        digit  = (seq_t)(offset - 6);      /* 1 … 9                       */
    }

    seq_t pow_val = pow10(length);        /* 10^length                   */
    seq_t repunit = pow_val - 1;          /* 10^length - 1               */
    repunit /= 9;                         /* (10^length-1)/9             */
    return repunit * digit;               /* final repdigit value         */
}

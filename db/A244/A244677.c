/*
 * A244677  The spiral of Champernowne, read along the East ray.
 * https://oeis.org/A244677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the digit at position (2*(n-1)+1)^2 - 7*(n-1) of Champernowne's
   constant (the digits 1,2,3,… concatenated).  The sequence is read
   along the east ray of the digit spiral. */
#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined for offset 0 */

    idx_t k = n - 1;                    /* 0‑based ring index   */
    seq_t two_k_plus1 = (seq_t)(2*k + 1);
    seq_t pos = two_k_plus1 * two_k_plus1 - (seq_t)7 * (seq_t)k;   /* 1‑based Champernowne position */

    /* locate the integer that contains this position */
    seq_t rem = pos;                     /* remaining position within the numbers */
    int d = 1;                           /* number of digits of the integer   */
    while (1) {
        seq_t block_count = (seq_t)9 * pow10((idx_t)(d - 1));   /* how many d‑digit numbers */
        seq_t block_digits = block_count * (seq_t)d;            /* digit count of this block  */
        if (rem <= block_digits)
            break;
        rem -= block_digits;
        d++;
    }

    seq_t base      = pow10((idx_t)(d - 1));   /* first number of this d‑digit block  */
    seq_t num_index = (rem - 1) / (seq_t)d;    /* zero‑based offset inside the block  */
    seq_t number    = base + num_index;        /* actual number                       */
    int  digit_pos  = (int)((rem - 1) % (seq_t)d); /* 0‑based index from left ­– 0 … d-1 */

    seq_t shift = (seq_t)(d - 1 - digit_pos);
    seq_t digit = (number / pow10((idx_t)shift)) % (seq_t)10;
    return digit;
}

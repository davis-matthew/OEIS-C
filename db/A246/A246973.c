/*
 * A246973  n^2 concatenated with (n+1)^2.
 * https://oeis.org/A246973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concat(n^2, (n+1)^2)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 207, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    seq_t n_val   = (seq_t)n;
    seq_t sq1     = n_val * n_val;          /* n^2          */
    seq_t np1     = n_val + 1;               /* n+1          */
    seq_t sq2     = np1 * np1;              /* (n+1)^2      */
    idx_t exp     = (idx_t)digit_len(sq2);   /* digits in (n+1)^2 */
    seq_t pow10d  = pow10(exp);              /* 10^exp       */
    return sq1 * pow10d + sq2;               /* concatenation*/
}

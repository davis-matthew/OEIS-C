/*
 * A246972  (n+1)^2 concatenated with n^2.
 * https://oeis.org/A246972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat((n+1)^2, n^2)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 206, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n) {
    seq_t n2  = (seq_t)n * (seq_t)n;          /* n^2 */
    seq_t np1 = (seq_t)(n + 1);               /* n+1 */
    seq_t np12 = np1 * np1;                   /* (n+1)^2 */
    int   d   = digit_len(n2);                /* number of decimal digits of n^2 */
    seq_t pow = pow10((idx_t)d);              /* 10^d */
    return np12 * pow + n2;                   /* concatenate */
}

/*
 * A269103  Number of n X 2 0..3 arrays with some element plus some
 *          horizontally or vertically adjacent neighbor totalling three no
 *          more than once.
 * https://oeis.org/A269103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=16, a(2)=180, a(n)=60*7^(n-3)*(8*n+5) for n ≥ 3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;                       /* not part of the sequence */
    if (n == 1) return (seq_t)16;
    if (n == 2) return (seq_t)180;

    /* for n ≥ 3 */
    seq_t exponent = (seq_t)(n - 3);            /* n-3 ≥ 0 */
    seq_t pow7    = big_pow((seq_t)7, exponent);
    seq_t mult    = (seq_t)8 * (seq_t)n + (seq_t)5;
    seq_t result  = (seq_t)60 * pow7;
    result *= mult;
    return result;
}

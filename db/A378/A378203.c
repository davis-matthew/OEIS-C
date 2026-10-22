/*
 * A378203  Number of palindromic n-ary words of length n that include the
 *          last letter of their respective alphabet.
 * https://oeis.org/A378203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^h(n) - (n-1)^h(n), where h(n) = ceil(n/2) for n > 0;
 *            a(0) = 1
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 46,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    /* ceiling of n/2 */
    idx_t h = (n + 1) / 2;
    seq_t pow_n  = big_pow((seq_t)n,        (seq_t)h);
    seq_t pow_nm = big_pow((seq_t)(n - 1),  (seq_t)h);
    return pow_n - pow_nm;
}

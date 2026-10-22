/*
 * A358599  Number of genetic relatives of a person M in a genealogical tree
 *          extending back n generations and where everyone has 5 children
 *          down to the generation of M.
 * https://oeis.org/A358599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + 5 * (10^n - 1) / 9
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute 2^n */
    seq_t pow2_n = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow2_n *= (seq_t)2;

    /* compute 10^n */
    seq_t pow10_n = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow10_n *= (seq_t)10;

    /* term2 = 5 * (10^n - 1) / 9 */
    seq_t term2 = pow10_n - (seq_t)1;
    term2 = term2 / (seq_t)9;
    term2 = term2 * (seq_t)5;

    return pow2_n + term2;
}

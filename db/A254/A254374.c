/*
 * A254374  Digital roots of centered pentagonal numbers (A005891).
 * https://oeis.org/A254374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = digital_root( (5*n*n - 5*n + 2) / 2 )
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute modulo 9 of the centered pentagonal number and its digital root */
    idx_t n_mod9 = n % 9U;
    idx_t n2_mod9 = (n_mod9 * n_mod9) % 9U;

    /* diff = n^2 - n (mod 9) */
    idx_t diff = (n2_mod9 + 9U - n_mod9) % 9U;

    /* E_mod9 = 5 * diff + 2 (mod 9) */
    idx_t term = (5U * diff) % 9U;
    idx_t E_mod9 = (term + 2U) % 9U;

    /* division by 2 modulo 9: multiply by inverse of 2 modulo 9, which is 5 */
    idx_t t_mod9 = (E_mod9 * 5U) % 9U;

    /* digital root: 0 maps to 9 */
    seq_t result = (t_mod9 == 0U) ? 9U : t_mod9;
    return result;
}

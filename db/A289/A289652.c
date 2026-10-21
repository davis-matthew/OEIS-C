/*
 * A289652  Catalan numbers - 2 (A120304) with first three terms changed to
 *          1,1,1.
 * https://oeis.org/A289652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 0,1,2 a(n) = Catalan(n) - 2 for n ≥ 3 Catalan
 *            numbers are computed by the recurrence C_{k+1} = C_k *
 *            2*(2k+1)/(k+2) with C_0 = 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the special first three terms */
    if (n == 0 || n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)1;

    /* compute Catalan(n) iteratively via the recurrence */
    seq_t C = (seq_t)1;          /* C_0 */
    for (idx_t k = 0; k < n; ++k)
    {
        /* numerator = C * 2 * (2*k + 1) */
        seq_t two_k_plus_one = (seq_t)2 * (seq_t)k + (seq_t)1;
        seq_t numerator = C * ((seq_t)2 * two_k_plus_one);

        /* denominator = k + 2 */
        seq_t denominator = (seq_t)k + (seq_t)2;

        C = numerator / denominator;   /* exact division */
    }

    /* subtraction of 2 gives the desired sequence entry */
    return C - (seq_t)2;
}

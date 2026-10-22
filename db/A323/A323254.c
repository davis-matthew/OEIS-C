/*
 * A323254  The determinant of an n X n Toeplitz matrix M(n) whose first row
 *          consists of successive positive integer numbers 2*n - 1, n - 1,
 *          ..., 1 and whose first column consists of 2*n - 1, 2*n - 2, ...,
 *          n.
 * https://oeis.org/A323254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * (n+1)^(n-1) + (n-1) * (((n-1)^(n-1) + (n+1)^(n-1)) /
 *            4) (The division by 4 is exact, and the term with (n-1)/4 is
 *            implemented as (n-1) * ( ... / 4 ).) Computes the OEIS sequence
 *            A323254
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* handle the special case n = 1 separately */
    if (n == 1)
        return (seq_t)1;

    seq_t n_seq          = (seq_t)n;
    seq_t n_minus1_seq   = (seq_t)(n - 1);
    seq_t n_plus1_seq    = (seq_t)(n + 1);

    /* compute (n+1)^(n-1) */
    seq_t pow1 = (seq_t)1;
    for (idx_t i = 0; i < n - 1; ++i)
        pow1 = pow1 * n_plus1_seq;

    /* compute (n-1)^(n-1) */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < n - 1; ++i)
        pow2 = pow2 * n_minus1_seq;

    /* first part: n * pow1 */
    seq_t term1 = n_seq * pow1;

    /* second part: (n-1) * (pow2 + pow1) / 4 */
    seq_t sum       = pow2 + pow1;
    seq_t sum_div4  = sum / 4;          /* exact division */
    seq_t term2     = n_minus1_seq * sum_div4;

    return term1 + term2;
}

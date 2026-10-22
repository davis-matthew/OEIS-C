/*
 * A372878  a(n) is the sum of all symmetric valleys in the set of flattened
 *          Catalan words of length n.
 * https://oeis.org/A372878
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^n*(2*n - 5) - 18*n^2 + 54*n - 27)/144
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 17, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Not defined for n < 4; return 0 for such inputs. */
    if (n < 4)
        return 0;

    seq_t n_s = (seq_t)n;

    /* Compute 3^n with arbitrary‑precision arithmetic. */
    seq_t pow3n = big_pow((seq_t)3, n_s);

    /* (2*n - 5) */
    seq_t two_n_minus5 = n_s * 2 - 5;

    /* 3^n * (2*n - 5) */
    seq_t term1 = pow3n * two_n_minus5;

    /* 18*n^2 */
    seq_t term2 = (seq_t)18 * n_s * n_s;

    /* 54*n */
    seq_t term3 = (seq_t)54 * n_s;

    /* Numerator = term1 - term2 + term3 - 27 */
    seq_t num = term1 - term2 + term3 - (seq_t)27;

    /* Final division by 144. */
    return num / (seq_t)144;
}

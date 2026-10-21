/*
 * A261869  First differences of A055615.
 * https://oeis.org/A261869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (n+1)*mu(n+1) - n*mu(n)
 * Range:     i8 n <= 64, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mu_of.h"

seq_t a(idx_t n)
{
    int mu_n   = mu_of(n);      /* μ(n)   */
    int mu_np1 = mu_of(n + 1);  /* μ(n+1) */

    return ((seq_t)(n + 1) * (seq_t)mu_np1) - ((seq_t)n * (seq_t)mu_n);
}

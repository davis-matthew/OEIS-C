/*
 * A384198  a(n) = 3^(n-3)*(binomial(n,3) + 3*binomial(n,2) + 9*n + 27).
 * https://oeis.org/A384198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n-3) * (C(n,3) + 3*C(n,2) + 9*n + 27), a(0)=1, a(1)=4,
 *            a(2)=16
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 35, u128 n <= 73,
 *            big any n
 * Helpers:   binom, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Handle the small indices where the exponent would be negative. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)16;

    /* Compute the polynomial part. */
    seq_t term3 = binom(n, (idx_t)3);
    seq_t term2 = binom(n, (idx_t)2);
    seq_t expr   = term3 + (seq_t)3 * term2 + (seq_t)9 * (seq_t)n + (seq_t)27;

    /* Raise 3 to the power n-3. */
    seq_t pow3_n_minus_3 = pow3(n - (idx_t)3);

    /* Multiply: 3^(n-3) * expr.  All arithmetic is done in seq_t,
       which may be 64‑bit or arbitrary precision depending on the
       build.  For a 64‑bit build the intermediate product fits
       within the type because 3^(n-3) grows slowly for n up to
       ~40 and a(n) is bounded by 2^64 up to that point. */
    return pow3_n_minus_3 * expr;
}

/*
 * A251694  a(n) = (2*n+1) * (3*n+1)^(n-2) * 4^n.
 * https://oeis.org/A251694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n+1) * (3*n+1)^(n-2) * 4^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Special cases: a(0)=1, a(1)=3 */

#include "../utils/seq_pow.h"   /* seq_pow(seq_t base, idx_t exp)  */
#include "../utils/seq_mul.h"   /* seq_mul(seq_t a, seq_t b)      */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly to avoid negative exponents. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* Compute (2*n + 1). */
    seq_t two_n_plus1 = (seq_t)(2 * n + 1);

    /* Compute (3*n + 1)^(n-2). */
    seq_t three_n_plus1 = (seq_t)(3 * n + 1);
    seq_t pow_3n1 = seq_pow(three_n_plus1, n - 2);

    /* Compute 4^n. */
    seq_t pow_4 = seq_pow((seq_t)4, n);

    /* Multiply the parts together using seq_mul to keep intermediates in seq_t. */
    seq_t prod = seq_mul(pow_3n1, pow_4);
    return seq_mul(two_n_plus1, prod);
}

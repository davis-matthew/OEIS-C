/*
 * A355999  Number of fixed orthoplex n-ominoes with cell centers determining
 *          (n-3)-space.
 * https://oeis.org/A355999
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-6) * n^(n-7) * (n-3)*(n-4)*(n-5)*(3n^3-17n^2+21n-78)
 *            / 3
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 9, u64 n <= 15, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* n is an index with offset 6; a(6) is the first term. */
/* This implementation works for all n >= 6. It handles the negative exponent
   case (n=6) by canceling with the product (n-3)*(n-4)*(n-5). */

#include "../utils/pow2.h"   /* pow2(idx_t exp) -> 2^exp */
#include "../utils/big_pow.h"/* big_pow(seq_t base, seq_t exp) */

seq_t a(idx_t n)
{
    /* Work only for indices >= offset 6; for smaller indices anything may be
       returned, but we provide 0 for clarity. */
    if (n < 6) return (seq_t)0;

    /* Promote to seq_t for all arithmetic. */
    seq_t N = (seq_t)n;

    /* 2^(n-6) */
    seq_t pow2val = pow2((idx_t)(n - 6));

    /* n^(n-7) – computed only when exponent is non‑negative.
       For n = 6, the exponent is -1; this case is handled by cancelling
       with the product below. */
    seq_t pow_n = (seq_t)1;
    if (n >= 7)
        pow_n = big_pow(N, (seq_t)(n - 7));

    /* (n-3)*(n-4)*(n-5) */
    seq_t prod = (N - (seq_t)3) * (N - (seq_t)4) * (N - (seq_t)5);

    /* If n = 6, divide by n to cancel the negative exponent. */
    if (n < 7)
        prod /= N;   /* Exact division: for n = 6, prod becomes 1 */

    /* Polynomial 3n^3 - 17n^2 + 21n - 78 */
    seq_t poly = (seq_t)3 * N * N * N;       /* 3n^3   */
    poly -= (seq_t)17 * N * N;              /* -17n^2 */
    poly += (seq_t)21 * N;                  /* +21n   */
    poly -= (seq_t)78;                      /* -78    */

    /* Assemble the product: 2^(n-6) * n^(n-7) * prod * poly / 3 */
    seq_t term = pow2val * pow_n;
    term *= prod;
    term *= poly;
    term /= (seq_t)3;      /* Exact division by 3 */

    return term;
}

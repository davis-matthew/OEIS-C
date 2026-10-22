/*
 * A361607  a(n) = n! * Sum_{k=0..n} binomial(n+(n-1)*k,n*k)/k!.
 * https://oeis.org/A361607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * Prod_{i=1}^{n-k} (n*k + i)
 *            equivalent to a(n) = n! * Sum_{k=0..n}
 *            binomial(n+(n-1)*k,n*k)/k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the utility for computing binomials */
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = binomial(n, k);          /* binomial(n,k) */
        idx_t terms_to_mul = n - k;           /* number of factors to multiply */
        idx_t base = n * k;                    /* starting value for the product */

        /* Multiply sequentially by (base + i) for i = 1 .. (n-k) */
        for (idx_t i = 1; i <= terms_to_mul; ++i) {
            term = term * (seq_t)(base + i);
        }

        sum = sum + term;
    }
    return sum;
}

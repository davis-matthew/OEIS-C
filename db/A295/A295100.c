/*
 * A295100  a(n) = n! * [x^n] exp(n*x)/(1 - 2*x).
 * https://oeis.org/A295100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} n^k * 2^(n-k)/k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow, fact_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/fact_upto.h"

/* Compute OEIS A295100: a(n) = n! * Sum_{k=0..n} n^k * 2^(n-k)/k! */
seq_t a(idx_t n) {
    /* t = n! * 2^n   (for k = 0) */
    seq_t t = fact_upto(n) * big_pow((seq_t)2, (seq_t)n);
    seq_t sum = t;

    for (idx_t k = 1; k <= n; ++k) {
        /* t = t * n / (k * 2)   (integer division) */
        seq_t numerator   = t * (seq_t)n;
        seq_t denominator = ((seq_t)k) * (seq_t)2;
        t = numerator / denominator;
        sum += t;
    }
    return sum;
}

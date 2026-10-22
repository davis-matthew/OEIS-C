/*
 * A345445  a(n) = n^n - (n+1)!/2.
 * https://oeis.org/A345445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^n - (n+1)!/2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/factorial.h"

seq_t a(idx_t n) {
    /* Compute n^n */
    seq_t pow_term = big_pow((seq_t)n, (seq_t)n);

    /* Compute (n+1)! / 2 */
    seq_t fact_term = factorial(n + 1);
    fact_term = fact_term / (seq_t)2;

    return pow_term - fact_term;
}

/*
 * A347061  Determinant of the (2n+1) X (2n+1) matrix with the (j,k)-entry
 *          (tan(Pi*(j-k)/(2n+1)))^2 (j,k = 0..2n).
 * https://oeis.org/A347061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n)*(2n+1)^(2n-1)*((2n+1)!!)^2 for n >= 1, a(0) = 0
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (2n+1)!! denotes the odd double factorial 1·3·5·…·(2n+1). */
#include <stdbool.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* 2n as seq_t */
    seq_t two_n = (seq_t)(2 * n);

    /* (2n+1)^(2n-1) */
    seq_t base = (seq_t)(2 * n + 1);
    idx_t exp = 2 * n - 1;
    seq_t pow_part = seq_pow(base, exp);

    /* (2n+1)!! = product of odd integers up to (2n+1) */
    seq_t odd_fact = (seq_t)1;
    for (idx_t i = 1; i <= n + 1; ++i) {
        odd_fact *= (seq_t)(2 * i - 1);
    }

    /* Square the odd double factorial */
    seq_t odd_fact_sq = odd_fact * odd_fact;

    /* Final result: (2n) * (2n+1)^(2n-1) * ((2n+1)!!)² */
    seq_t result = two_n * pow_part;
    result *= odd_fact_sq;

    return result;
}

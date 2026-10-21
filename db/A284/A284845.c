/*
 * A284845  Number of permutations on [n+4] with no circular 4-successions.
 * https://oeis.org/A284845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   factorial, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A284845: Number of permutations on [n+4] with no circular 4-successions.
 *
 * Formula:
 *     a(n) = (n+4) * Sum_{j=0..n} (-1)^j * binomial(n,j) * (n-j+3)!.
 *
 * The implementation uses the recurrence
 *     term_j = term_{j-1} * (n-j+1) / j / (n-j+4)
 * with term_0 = (n+3)! to compute the summand efficiently.
 * It relies on the utility functions `factorial` and `mul_small`.
 */

#include <stdlib.h>
#include "../utils/factorial.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Ensure n ≥ 1 (offset 1). For n = 0 the formula would give 24, which is not part of the sequence. */
    if (n == 0)
        return 0;  /* defensive, though this case is not expected */

    /* term0 = (n+3)! */
    seq_t term_prev = factorial(n + 3);

    /* sum = Σ_{j=0..n} (-1)^j * term_j, starting with j = 0. */
    seq_t sum = term_prev;

    for (idx_t j = 1; j <= n; ++j) {
        /* Compute term_j from term_{j-1}. */
        /* term_j = term_{j-1} * (n-j+1) / j / (n-j+4) */
        seq_t temp = mul_small(term_prev, (unsigned int)(n - j + 1));
        temp = temp / (seq_t)j;                     /* division by j */
        seq_t term = temp / (seq_t)(n - j + 4);      /* division by (n-j+4) */

        /* Add or subtract according to the parity of j. */
        if (j & 1U)
            sum -= term;   /* odd j → subtract */
        else
            sum += term;   /* even j → add */

        term_prev = term;
    }

    /* Multiply the accumulated sum by (n+4). */
    return mul_small(sum, (unsigned int)(n + 4));
}

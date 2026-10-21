/*
 * A290028  Number of 5-cycles in the n-halved cube graph.
 * https://oeis.org/A290028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^n*binomial(n,4)*(29*n-86)/5
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* For n < 4 the formula gives 0 */
    if (n < 4) return 0;

    /* 2^n */
    seq_t power = pow2(n);

    /* binomial(n,4) */
    seq_t binom_val = binomial(n, 4);

    /* (29*n - 86) */
    seq_t part = 29 * (seq_t)n - (seq_t)86;

    /* divide by 5 on whichever factor is divisible */
    if (binom_val % 5 == 0) {
        binom_val /= 5;
    } else {
        part /= 5;
    }

    /* compute a(n) */
    seq_t result = power;
    result *= binom_val;
    result *= part;
    result *= (seq_t)3;

    return result;
}

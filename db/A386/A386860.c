/*
 * A386860  The total number of big descents in all parking functions of
 *          length n.
 * https://oeis.org/A386860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n-1,2)*(n+1)^(n-2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n <= 2)
        return 0;
    seq_t binom = binomial(n-1, 2);
    seq_t base = (seq_t)(n + 1);
    seq_t exp  = (seq_t)(n - 2);
    seq_t pow  = big_pow(base, exp);
    return binom * pow;
}

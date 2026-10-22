/*
 * A393238  Number of transitive relations on an n-set with at most two
 *          ordered pairs.
 * https://oeis.org/A393238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n,0) + binomial(n,1) + 7*binomial(n,2) +
 *            12*binomial(n,3) + 12*binomial(n,4)
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = binomial(n, 0);
    if (n >= 1) sum += binomial(n, 1);
    if (n >= 2) sum += (seq_t)7 * binomial(n, 2);
    if (n >= 3) sum += (seq_t)12 * binomial(n, 3);
    if (n >= 4) sum += (seq_t)12 * binomial(n, 4);
    return sum;
}

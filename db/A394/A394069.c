/*
 * A394069  Number of unsatisfiable 3-SAT formulas with 2 variables and n
 *          clauses in the multiset clause model.
 * https://oeis.org/A394069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+19,n) - 4*binomial(n+15,n) +
 *            4*binomial(n+12,n) + 2*binomial(n+11,n) - 4*binomial(n+9,n) +
 *            binomial(n+7,n);
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t val = binomial(n + 19, 19);
    val += (seq_t)4 * binomial(n + 12, 12);
    val += (seq_t)2 * binomial(n + 11, 11);
    val += binomial(n + 7, 7);
    val -= (seq_t)4 * binomial(n + 15, 15);
    val -= (seq_t)4 * binomial(n + 9, 9);
    return val;
}

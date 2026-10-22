/*
 * A397222  Number of satisfiable 3-SAT formulas with 2 variables and n
 *          clauses in the multiset clause model.
 * https://oeis.org/A397222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*binomial(n+15,n) - 4*binomial(n+12,n) -
 *            2*binomial(n+11,n) + 4*binomial(n+9,n) - binomial(n+7,n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1; behaviour for n==0 is unspecified */
    if (n == 0)
        return 0;
    seq_t t1 = binomial(n + 15, n);
    seq_t t2 = binomial(n + 12, n);
    seq_t t3 = binomial(n + 11, n);
    seq_t t4 = binomial(n + 9, n);
    seq_t t5 = binomial(n + 7, n);
    return 4 * t1 - 4 * t2 - 2 * t3 + 4 * t4 - t5;
}

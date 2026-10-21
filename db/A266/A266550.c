/*
 * A266550  Independence number, upper domination number, and upper
 *          irredundance number of the n-Mycielski graph.
 * https://oeis.org/A266550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n = 1, 2; for n > 2: a(n) = -1 + 3 * 2^(n-3) (OEIS
 *            A266550)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* seq_t and idx_t are defined by the build (see build headers) */
seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)1;

    /* 2^(n-3) */
    seq_t pow = pow2(n - 3);

    /* 3 * pow - 1 */
    seq_t result = ((seq_t)3 * pow) - (seq_t)1;

    return result;
}

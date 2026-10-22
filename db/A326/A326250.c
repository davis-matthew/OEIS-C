/*
 * A326250  Number of weakly nesting simple graphs with vertices {1..n}.
 * https://oeis.org/A326250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2^(n choose 2) - Catalan(n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* Exponent is n*(n-1)/2 */
    seq_t exp = ((seq_t)n * ((seq_t)n - 1)) / 2;

    /* Total number of simple graphs on n vertices */
    seq_t total = big_pow((seq_t)2, exp);

    /* Catalan number for n */
    seq_t cat = catalan(n);

    /* a(n) = total graphs - Catalan(n) */
    return total - cat;
}

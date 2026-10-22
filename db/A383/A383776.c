/*
 * A383776  a(n) = (11*n + 3 + 6/(n+2)) * Catalan(n).
 * https://oeis.org/A383776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 3 + 6/(n+2)) * Catalan(n) (OEIS A383776)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic */
    seq_t sn = (seq_t)n;

    /* Catalan numbers are provided by utils/catalan.h */
    seq_t cat = catalan(n);

    /* Compute 11*n + 3 */
    seq_t num = 11 * sn + 3;

    /* a(n) = num * cat + (6 * cat) / (n + 2)   (avoids overflow) */
    seq_t part1 = num * cat;
    seq_t part2 = (cat * 6) / (sn + 2);

    return part1 + part2;
}

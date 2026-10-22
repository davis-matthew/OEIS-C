/*
 * A321050  Number of permutations of [n] with no fixed points where the
 *          maximal displacement of an element equals four.
 * https://oeis.org/A321050
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A259777(n) - A079997(n) Count derangements of [n] with
 *            maximal displacement exactly 4
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 38, u128 n <= 74,
 *            big any n
 * Helpers:   count_derangements_match
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/count_derangements_match.h"

seq_t a(idx_t n)
{
    if (n < 4) return 0;                 /* offset at 4 */

    seq_t tot4 = count_derangements_match(n, 4);
    seq_t tot3 = count_derangements_match(n, 3);

    return tot4 - tot3;
}

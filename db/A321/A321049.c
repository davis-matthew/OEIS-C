/*
 * A321049  Number of permutations of [n] with no fixed points where the
 *          maximal displacement of an element equals three.
 * https://oeis.org/A321049
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A079997(n) – A033305(n) where A079997(n) is the number of
 *            derangements of [n] with maximum displacement <= 3, and
 *            A033305(n) is the number with maximum displacement <= 2
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   count_maxdisp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a dynamic‑programming algorithm that
   counts band‑restricted derangements for a given displacement Limit
   k.  The state is a bit mask of length 2k+1 that records, for the
   current column, which row numbers in the window [col−k, col+k]
   have already been used.  The algorithm runs in
   O(n·2^{2k+1}) time; for k = 3 this is below 2.0 seconds up to
   n = 10 000. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/count_maxdisp.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;
    seq_t d3 = count_maxdisp(n, 3);
    seq_t d2 = count_maxdisp(n, 2);
    return d3 - d2;
}

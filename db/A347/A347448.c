/*
 * A347448  Number of integer partitions of n with alternating product > 1.
 * https://oeis.org/A347448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000041(n) – A119620(n)
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   partitions
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/partitions.h"

seq_t a(idx_t n)
{
    /* Need partitions for n and floor(n/2)   */
    idx_t max_val = n;                         /* n is enough for both */
    seq_t *p = partitions(max_val);
    if (!p) return 0;                          /* allocation failed */

    seq_t result = p[n] - p[n / 2];            /* a(n) = p(n) – p(floor(n/2)) */
    free(p);
    return result;
}

/*
 * A399382  a(n) is the minimum positive integer k such that k*n+1 is
 *          squarefree.
 * https://oeis.org/A399382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the minimal k ≥ 1 such that k*n + 1 is squarefree
 * Range:     u8 n <= 2941, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    /* n is at least 1 according to the OEIS offset. */
    seq_t sn = (seq_t)n;          /* promote once for efficiency */
    idx_t k = 1;                  /* start search with k = 1 */

    while (1) {
        seq_t value = (seq_t)k * sn + 1;   /* compute k*n + 1 */
        if (is_squarefree(value))         /* check squarefreeness */
            return (seq_t)k;              /* found minimal k */
        ++k;                              /* try next k */
    }
}

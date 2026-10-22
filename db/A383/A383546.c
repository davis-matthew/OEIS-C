/*
 * A383546  a(n) is the largest number k such that i*n - 1 is squarefree for
 *          all 1 <= i <= k, or 0 if no such number exists.
 * https://oeis.org/A383546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = largest k such that i*n-1 is squarefree for all 1 <= i <=
 *            k, or 0 if no such k exists (i.e. n = 1 or n-1 is nonsquarefree)
 * Range:     u8 n <= 11, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* External utility for testing squarefreeness: */
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    /* Special case: n = 1 */
    if (n == 1) {
        return 0;
    }

    /* If n-1 is not squarefree, a(n) = 0 */
    if (!is_squarefree((seq_t)n - 1)) {
        return 0;
    }

    /* Search for the largest k */
    for (idx_t i = 1; ; ++i) {
        seq_t v = (seq_t)i * (seq_t)n - 1;
        if (!is_squarefree(v)) {
            return (seq_t)(i - 1);
        }
    }
}

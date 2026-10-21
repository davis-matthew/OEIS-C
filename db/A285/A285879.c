/*
 * A285879  Number of odd squarefree numbers <= n.
 * https://oeis.org/A285879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of odd squarefree integers k with 1 ≤ k ≤ n
 * Range:     u8 n <= 629, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: iterate over all odd numbers up to n, test squarefreeness with the supplied helper. */

#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if ((i & 1) && is_squarefree((seq_t)i))
            ++count;
    }
    return count;
}

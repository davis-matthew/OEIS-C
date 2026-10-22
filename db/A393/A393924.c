/*
 * A393924  Numbers that are either squarefree or squareful.
 * https://oeis.org/A393924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th positive integer that is either squarefree or
 *            squareful
 * Range:     u8 n <= 181, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree_or_squareful
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_squarefree_or_squareful.h"

/* Return the n-th term of A393924 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n = 0; return 0 safely */

    uint64_t candidate = 1;
    idx_t    count = 0;

    while (count < n) {
        if (is_squarefree_or_squareful(candidate)) {
            ++count;
            if (count == n) return (seq_t)candidate;
        }
        ++candidate;
    }

    /* Should never reach here. */
    return 0;
}

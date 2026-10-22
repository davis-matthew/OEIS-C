/*
 * A308454  Number of integer-sided triangles with perimeter n whose largest
 *          side length is squarefree.
 * https://oeis.org/A308454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of integer-sided triangles with perimeter n whose
 *            largest side is squarefree
 * Range:     u8 n <= 138, u16 n <= 2276, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    /* For n < 3 there are no integer-sided triangles. */
    if (n < 3) return 0;

    /* Longest side c ranges from ceil(n/3) to floor((n-1)/2). */
    idx_t start = (n + 2) / 3;          /* ceil(n/3) */
    idx_t end   = (n - 1) / 2;          /* floor((n-1)/2) */

    if (start > end) return 0;

    seq_t result = 0;

    for (idx_t c = start; c <= end; ++c)
    {
        /* Only squarefree largest side allowed. */
        if (!is_squarefree((seq_t)c)) continue;

        idx_t s = n - c;            /* sum of the other two sides */
        idx_t lower = s - c;        /* minimal a, ensures b ≤ c */
        idx_t upper = s >> 1;       /* floor(s/2), maximal a because a ≤ b */

        if (upper >= lower)
        {
            idx_t cnt = upper - lower + 1;
            result += (seq_t)cnt;
        }
    }

    return result;
}

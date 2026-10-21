/*
 * A283190  a(n) is the number of different values n mod k for 1 <= k <=
 *          floor(n/2).
 * https://oeis.org/A283190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = number of distinct values n mod k for 1 <= k <=
 *            floor(n/2)
 * Range:     u8 n <= 1120, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n < 2) return 0;                      /* floor(n/2) == 0 -> no k */

    idx_t m = n / 2;                          /* floor(n/2) */
    /* Allocate a byte array to mark residues that already appeared.
       Index r corresponds to residue r.  Since r < k <= m,
       the highest possible residue is m-1. */
    unsigned char *seen = (unsigned char *)calloc((size_t)m, 1u);
    if (!seen) return 0;                      /* allocation failure → 0 */

    for (idx_t k = 1; k <= m; ++k)
    {
        idx_t r = n % k;                      /* 0 <= r < k <= m */
        if (!seen[r])
        {
            seen[r] = 1;
            ++count;
        }
    }

    free(seen);
    return count;
}

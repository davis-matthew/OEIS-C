/*
 * A335012  Number of different remainders when the first n terms of 1, 11,
 *          111, 1111, ... are divided by n.
 * https://oeis.org/A335012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = number of distinct residues among (10^k-1)/9 modulo n for
 *            k = 1..n. This is the OEIS sequence A335012
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not in OEIS offset, but safe */
    /* allocate visited array of n bytes (0 = unseen, 1 = seen) */
    bool *seen = (bool *)calloc((size_t)n, sizeof(bool));
    if (!seen) return 0;           /* allocation failure, should not happen */
    idx_t value = 0;               /* will hold current remainder (10^k-1)/9 mod n */
    seq_t distinct = 0;            /* number of distinct remainders */

    for (idx_t k = 1; k <= n; ++k)
    {
        if (k == 1)
            value = 1 % n;           /* first term = 1 */
        else
            value = (value * 10 + 1) % n;   /* recurrence: next term = prev*10+1 */

        if (!seen[value])
        {
            seen[value] = true;
            ++distinct;
        }
    }
    free(seen);
    return distinct;
}

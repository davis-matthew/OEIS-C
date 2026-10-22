/*
 * A389922  Standard composition numbers of integer compositions with minimum
 *          = length.
 * https://oeis.org/A389922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) equals the index k of the n-th composition in standard
 *            order (graded reverse‑lexicographic) whose minimum part equals
 *            the length of the composition. The index is determined by the
 *            binary expansion of k: take the positions of the 1‑bits
 *            (counting from 1 starting at the least significant bit),...
 * Range:     u8 n <= 10, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* unused, offset is 1 */

    idx_t count = 0;        /* number of valid compositions seen */
    for (idx_t k = 1;; ++k)   /* enumerate compositions in standard order */
    {
        /* collect positions of 1-bits in the binary expansion of k,
           counting from 1 starting at the least significant bit */
        int pos[65];        /* maximum 64 bits of k + prepended 0 */
        int sz = 1;         /* number of entries in pos (including 0) */
        pos[0] = 0;         /* prepend 0 */

        idx_t tmp = k;
        int j = 0;
        while (tmp) {
            if (tmp & (idx_t)1) pos[sz++] = j + 1;   /* 1-based position */
            tmp >>= 1;
            ++j;
        }

        int len = sz - 1;    /* number of parts in the composition */

        seq_t val   = 0;     /* integer value for this composition */
        idx_t cum   = 0;     /* cumulative sum of parts */
        idx_t minp  = (idx_t)-1;   /* large initial value */

        /* traverse the parts in forward order (from first to last) to
           form the integer.  For the required mapping each part is
           processed in reverse order of appearance, which is achieved
           by starting with the first difference and accumulating. */
        for (int i = 1; i < sz; ++i) {
            idx_t part = (idx_t)(pos[i] - pos[i-1]);
            if (part < minp) minp = part;
            cum += part;
            /* set bit at position (cum - 1) */
            val |= ((seq_t)1 << (cum - 1));
        }

        if (minp == (idx_t)len) {   /* composition satisfies min = length */
            ++count;
            if (count == n)
                return val;        /* return the desired term */
        }
    }
}

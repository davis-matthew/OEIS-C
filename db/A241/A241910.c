/*
 * A241910  After a(1)=0, numbers 0 .. bigomega(n)-1, followed by numbers 0 ..
 *          bigomega(n+1)-1, etc., where bigomega(n)=A001222(n) is the number
 *          of prime factors of n (with repetition).
 * https://oeis.org/A241910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A241910
 * Formula: a(1) = 0,
 * for n ≥ 2, a(n) is the offset (starting at 0) of the element at
 * position n in the concatenation of rows, where row k (k ≥ 2)
 * consists of the numbers 0,1,…,bigomega(k)-1 (bigomega = A001222).
 */

#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* a(1) is defined as 0 */
    if (n <= 1)
        return 0;

    /* After the first term, the flattened sequence starts at n = 2
     * and each entry corresponds to a 0‑based index `target`. */
    idx_t target = n - 2;          /* 0‑based index after a(1) */

    idx_t cumulative = 0;          /* sum of bigomega values of all
                                     previous rows (starting with row 2) */
    idx_t row = 2;                 /* current row being examined */

    while (1) {
        seq_t w = bigomega(row);           /* bigomega(row) fits in seq_t */
        idx_t w_idx = (idx_t)w;            /* convert to a native index type */

        /* If the target lies within this row, stop; otherwise
         * accumulate and proceed to the next row.        */
        if (cumulative + w_idx > target)
            break;

        cumulative += w_idx;
        ++row;
    }

    /* Compute the position within the current row. */
    idx_t offset = target - cumulative;
    return (seq_t)offset;
}

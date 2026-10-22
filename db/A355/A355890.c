/*
 * A355890  Let s(k) = A052551(k), and write down s(0) consecutive integers
 *          beginning with 0, skip one integer, continue with s(1) consecutive
 *          numbers, skip one integer, then s(2) consecutive numbers, skip one
 *          integer, then s(3) consecutive numbers, and so on.
 * https://oeis.org/A355890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A355890
 *
 *  Construction:  Let s(k) = 2^{⌊k/2⌋+1} – 1 (A052551(k)).
 *  Write s(0) consecutive integers beginning at 0, skip one integer,
 *  write s(1) consecutive integers, skip one, and so on.
 *
 *  Implementation:  Iteratively accumulate full groups until the
 *  desired index is reached.  The current value of the sequence and
 *  the accumulated number of terms are maintained.  The group lengths
 *  grow exponentially, so only O(log n) groups are examined.
 *
 *  Only integer arithmetic is used.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t sum     = 0;          /* number of terms produced so far   */
    idx_t group   = 0;          /* current group index               */
    seq_t start   = 0;          /* first value of the current group  */

    while (1) {
        /* Length of the current group: 2^{⌊group/2⌋+1} – 1.  */
        idx_t e   = (group / 2) + 1;
        idx_t len = ((idx_t)1 << e) - 1;   /* safe: e <= 64 for all required n */

        /* If the whole group fits before index n, skip it. */
        if (sum + len <= n) {
            sum   += len;
            start += (seq_t)(len + 1);  /* move to start of next group (skip one) */
            group += 1;
            continue;
        }
        /* n lies inside this group. */
        idx_t offset = n - sum;        /* position inside the group */
        return start + (seq_t)offset;
    }
}

/*
 * A373954  Excess run-compression of standard compositions. Sum of all parts
 *          minus sum of compressed parts of the n-th integer composition in
 *          standard order.
 * https://oeis.org/A373954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A029837(n) - A373953(n) Standard order of compositions
 *            (graded reverse‑lexicographic). For n >= 0, return the excess of
 *            uncompressed versus compressed part sums of the n‑th composition
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute the excess of the n-th composition in standard order.        */
seq_t a(idx_t n)
{
    /* Rank 0 corresponds to the empty composition.   */
    if (n == 0) return 0;

    /* Determine the weight S (sum of the composition).  */
    idx_t tmp = n;
    unsigned S = 0;                       /* 1 <= S <= 64 */
    while (tmp) { ++S; tmp >>= 1; }

    /* Compute the offset of n within the block of weight S. */
    idx_t start = ((idx_t)1) << (S - 1);
    idx_t offset = n - start;              /* 0 <= offset < 2^(S-1) */
    unsigned m = S - 1;                    /* number of cut bits */

    /* The binary pattern of cuts is exactly the offset.             */
    idx_t bits = offset;                   /* bit i = cut after unit i+1 */

    /* Totals for the sum of all parts and the compressed sum.       */
    seq_t total   = 0;
    seq_t comp    = 0;
    seq_t cur_part = 0;                /* running length of current part       */
    seq_t prev_part = 0;               /* previous part length                  */
    bool   first = true;                /* true until at least one part seen    */

    for (unsigned i = 0; i < S; ++i) {
        cur_part += (seq_t)1;               /* add one unit to current part */

        /* If a cut occurs after unit i+1, emit the current part. */
        if (i < m && ((bits >> i) & 1ull)) {
            seq_t part = cur_part;
            total += part;
            if (first || part != prev_part) {
                comp += part;
                first = false;
            }
            prev_part = part;
            cur_part = 0;
        }
    }

    /* Emit the final part (after the last unit). */
    if (cur_part != 0) {
        seq_t part = cur_part;
        total += part;
        if (first || part != prev_part) {
            comp += part;
            first = false;
        }
        prev_part = part;
    }

    return total - comp;
}

/*
 * A389600  Run-compression sum (A373953) minus number of maximal runs in the
 *          n-th composition in standard order.
 * https://oeis.org/A389600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A373953(n) – A124767(n) where A373953(n) is the sum of
 *            the run‑compressed parts of the n‑th composition in standard
 *            order, and A124767(n) is the number of maximal runs in that
 *            composition. The compositions are obtained from the integer n
 *            itself: take the set of positions of 1's in the reve...
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The empty composition corresponds to n = 0. */
    if (n == 0)
        return 0;

    /* positions[0] = 0; subsequent positions are the indices (1‑based)
       of the set bits of n, which are exactly the positions in the
       reversed binary expansion. */
    idx_t positions[65];
    idx_t pos_cnt = 0;
    positions[pos_cnt++] = 0;          /* prepend 0 */

    idx_t bit_pos = 1;                 /* 1‑based bit index in reversed binary */
    idx_t temp = n;
    while (temp) {
        if (temp & 1)
            positions[pos_cnt++] = bit_pos;
        temp >>= 1;
        bit_pos++;
    }

    /* A composition has at least one part only if pos_cnt > 1. */
    if (pos_cnt <= 1)
        return 0;

    seq_t compressed_sum = 0;
    seq_t run_count      = 0;
    seq_t prev_part      = 0;
    bool   first_part    = true;

    /* Process the differences in reverse order, which gives the
       composition parts from left to right. */
    for (idx_t j = pos_cnt - 1; j > 0; --j) {
        idx_t diff = positions[j] - positions[j - 1];  /* part size (positive) */

        if (first_part) {
            /* First part starts a new run. */
            run_count++;
            compressed_sum += (seq_t)diff;
            prev_part = diff;
            first_part = false;
        } else {
            if (diff != prev_part) {
                /* A new run starts. */
                run_count++;
                compressed_sum += (seq_t)diff;
                prev_part = diff;
            }
            /* If diff == prev_part, we stay in the same run and
               do not count it or add to the compressed sum. */
        }
    }

    /* a(n) = sum of compressed parts – number of maximal runs. */
    return compressed_sum - run_count;
}

/*
 * A374683  Irregular triangle read by rows where row n lists the leaders of
 *          strictly increasing runs in the n-th composition in standard
 *          order.
 * https://oeis.org/A374683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = the n-th entry of the triangle defined by * the leaders
 *            of strictly increasing runs in the k-th composition * in
 *            standard order (graded reverse‑lexicographic, A066099). * The
 *            function iterates k from 0 upward, generates the * composition
 *            corresponding to k, extracts the leaders of * strict...
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t target = n;          /* desired flattened index */
    idx_t count = 0;           /* number of leaders seen so far */

    for (idx_t k = 0;; ++k) {
        /* --- generate composition for k --- */
        /* bits of k (least significant bit has index 1)  */
        idx_t positions[64];
        int pos_len = 0;
        idx_t temp = k;
        idx_t bit_pos = 1;
        while (temp) {
            if (temp & 1U) {
                positions[pos_len++] = bit_pos;
            }
            temp >>= 1U;
            ++bit_pos;
        }
        /* differences of successive positions, with leading 0 */
        seq_t parts[64];
        int part_len = 0;
        idx_t prev = 0;
        for (int i = 0; i < pos_len; ++i) {
            idx_t pos = positions[i];
            parts[part_len++] = (seq_t)(pos - prev);
            prev = pos;
        }
        /* reverse parts to obtain the composition in standard order */
        seq_t comp[64];
        for (int i = 0; i < part_len; ++i) {
            comp[i] = parts[part_len - 1 - i];
        }

        /* --- extract leaders of strictly increasing runs --- */
        if (part_len > 0) {
            seq_t prev_val = 0;
            for (int i = 0; i < part_len; ++i) {
                seq_t val = comp[i];
                if (i == 0 || prev_val >= val) {           /* start of a run */
                    if (count == target)
                        return val;                       /* found the answer */
                    ++count;
                }
                prev_val = val;
            }
        }
    }

    /* unreachable */
    return 0;
}

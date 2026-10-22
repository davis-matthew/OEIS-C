/*
 * A374515  Irregular triangle read by rows where row n lists the leaders of
 *          anti-runs in the n-th composition in standard order.
 * https://oeis.org/A374515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A374515: Return the nth leader of the flattened triangle
   of leaders of anti‑runs in the k‑th composition enumerated in
   standard order (A066099).  The code constructs compositions on
   the fly by examining the binary representation of the index k. */
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n) {
    idx_t count = 0;                 /* number of leaders produced so far */
    for (idx_t k = 0; ; ++k) {       /* iterate over compositions in order */
        /* collect positions of 1-bits in reversed binary of k */
        idx_t pos_arr[65];
        idx_t poslen = 0;
        idx_t temp = k;
        idx_t pos = 1;               /* 1‑based position of least significant bit */
        while (temp) {
            if (temp & 1U) pos_arr[poslen++] = pos;
            temp >>= 1;
            ++pos;
        }
        if (poslen == 0) continue;   /* empty composition (k = 0) */

        /* compute parts of the composition */
        idx_t comp[65];
        comp[0] = pos_arr[0];        /* first part */
        for (idx_t i = 1; i < poslen; ++i)
            comp[i] = pos_arr[i] - pos_arr[i-1];

        /* reverse the parts to obtain the correct order */
        for (idx_t i = 0; i < poslen / 2; ++i) {
            idx_t tmp = comp[i];
            comp[i] = comp[poslen - 1 - i];
            comp[poslen - 1 - i] = tmp;
        }

        /* emit leaders of anti‑runs */
        seq_t val = (seq_t)comp[0];
        if (count == n) return val;
        ++count;
        for (idx_t i = 1; i < poslen; ++i) {
            if (comp[i] == comp[i - 1]) {
                val = (seq_t)comp[i];
                if (count == n) return val;
                ++count;
            }
        }
    }
}

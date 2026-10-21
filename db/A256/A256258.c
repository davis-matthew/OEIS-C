/*
 * A256258  Triangle read by rows in which the row lengths are the terms of
 *          A011782 and row n lists the terms of A016969 except for the right
 *          border which gives the positive terms of A000225.
 * https://oeis.org/A256258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 2^r – 1 when n is the last element of row r (i.e. n =
 *            2^(r–1)); otherwise a(n) = 5 + 6*(pos–1), where pos is the
 *            1‑based column within that row
 * Range:     u8 n <= 106, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) /* safety for indices below the offset */  return (seq_t)0;
    idx_t r   = 1;          /* current row number  */
    idx_t cum = 0;          /* total terms before the row */

    while (1) {
        idx_t len = (r <= 2) ? 1 : (1ULL << (r - 2));   /* length of row r */

        if (n <= cum + len) {                         /* n lies in row r */
            idx_t pos = n - cum;                      /* 1‑based column */

            if (pos < len) {                          /* interior element */
                seq_t val = (seq_t)5 + (seq_t)6 * (seq_t)(pos - 1);
                return val;
            } else {                                  /* right border element */
                seq_t val = pow2(r);                  /* 2^r */
                return val - (seq_t)1;                /* 2^r – 1 */
            }
        }

        cum += len;                                    /* move to next row */
        ++r;
    }
}

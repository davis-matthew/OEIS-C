/*
 * A328870  Numbers whose lengths of runs of 1's in their reversed binary
 *          expansion are not weakly increasing.
 * https://oeis.org/A328870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A328870: numbers whose lengths of runs of consecutive 1's
   in the reversed binary representation are not weakly increasing.
   (Computed by brute‑force search.) */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* undefined for n = 0 */

    idx_t found = 0;
    seq_t m = 1;                    /* start checking from 1 */

    for (;;) {
        seq_t temp = m;
        int prevLen = -1;           /* length of previous run of 1's */
        int currLen = 0;            /* length of current run */
        bool qualifies = false;     /* does this m satisfy the condition? */

        /* Scan bits from least significant to most significant. */
        while (temp > 0) {
            if (temp & 1) {
                /* inside a run of 1's */
                currLen++;
            } else {
                /* end of a run */
                if (currLen > 0) {
                    if (prevLen != -1 && currLen < prevLen) {
                        qualifies = true;
                        break;
                    }
                    prevLen = currLen;
                    currLen = 0;
                }
            }
            temp >>= 1;
        }

        /* Process a run that ends at the most significant bit. */
        if (!qualifies && currLen > 0) {
            if (prevLen != -1 && currLen < prevLen)
                qualifies = true;
        }

        if (qualifies) {
            ++found;
            if (found == n)
                return m;
        }

        ++m;
    }
}

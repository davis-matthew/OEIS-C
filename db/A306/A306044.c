/*
 * A306044  Powers of 2, 3 and 5.
 * https://oeis.org/A306044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 64, u64 n <= 131,
 *            u128 n <= 263, big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th smallest element of the unrestricted union
 * of the sequences 2^i , 3^j , 5^k (i, j, k ≥ 0).
 */
#include <stdbool.h>
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Current values for the three geometric progressions. */
    seq_t val2 = (seq_t)1;
    seq_t val3 = (seq_t)1;
    seq_t val5 = (seq_t)1;

    /* Flags indicating that the corresponding progression has
     * already reached the outer range of the type and should no
     * longer be advanced.  In this case the value is kept as a
     * “sentinel” equal to the maximum representable value, which
     * is larger than any true finite term. */
    bool over2 = false;
    bool over3 = false;
    bool over5 = false;

    seq_t result = 0;

    for (idx_t i = (idx_t)1; i <= n; ++i) {
        /* Find the minimum of the three candidate values. */
        seq_t mn = val2;
        if (val3 < mn) mn = val3;
        if (val5 < mn) mn = val5;

        result = mn;

        /* Advance each progression that produced the minimum. */
        if (!over2 && val2 == mn) {
            seq_t nxt = seq_mul(val2, (seq_t)2);
            if (nxt < val2) {            /* overflow detected */
                over2 = true;
                val2 = (seq_t)-1;        /* sentinel (max value) */
            } else {
                val2 = nxt;
            }
        }
        if (!over3 && val3 == mn) {
            seq_t nxt = seq_mul(val3, (seq_t)3);
            if (nxt < val3) {
                over3 = true;
                val3 = (seq_t)-1;
            } else {
                val3 = nxt;
            }
        }
        if (!over5 && val5 == mn) {
            seq_t nxt = seq_mul(val5, (seq_t)5);
            if (nxt < val5) {
                over5 = true;
                val5 = (seq_t)-1;
            } else {
                val5 = nxt;
            }
        }
    }

    return result;
}

/*
 * A296892  Numbers whose base-13 digits d(m), d(m-1), ..., d(0) have #(pits)
 *          > #(peaks); see Comments.
 * https://oeis.org/A296892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   computed by brute‑force search; sequence of integers whose
 *            base‑13 digits contain more pits than peaks
 * Range:     u8 n <= 12, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   A pit is an index i such that d(i‑1) > d(i) < d(i+1);
   A peak is an index i such that d(i‑1) < d(i) > d(i+1).
   Here d(0) is the least significant digit.
*/

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    idx_t found = 0;              /* how many terms found so far */
    seq_t  val   = 1;              /* candidate value, start from 1 */

    /* temporary buffer for base‑13 digits */
    uint8_t digits[64];            /* 64 digits are more than enough for 128‑bit integers */

    while (found < n) {
        /* decompose 'val' into base‑13 digits, least significant first */
        seq_t tmp = val;
        size_t len = 0;
        if (tmp == 0) {            /* handle zero explicitly (won't happen but safe) */
            digits[0] = 0;
            len = 1;
        } else {
            while (tmp > 0) {
                digits[len++] = (uint8_t)(tmp % 13);
                tmp /= 13;
            }
        }

        /* count pits and peaks */
        int pits   = 0;
        int peaks  = 0;
        if (len >= 3) {           /* at least one interior digit is required */
            for (size_t i = 1; i + 1 < len; ++i) {
                uint8_t a = digits[i - 1];
                uint8_t b = digits[i];
                uint8_t c = digits[i + 1];
                if (a > b && c > b)
                    pits++;
                else if (a < b && c < b)
                    peaks++;
            }
        }

        if (pits > peaks) {
            found++;
            if (found == n)
                return val;
        }

        val++;                   /* next candidate */
    }

    /* This line should never be reached because the sequence is infinite */
    return 0;
}

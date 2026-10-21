/*
 * A297044  Number of pieces in the list d(m), d(m-1), ..., d(0) of base-16
 *          digits of n; see Comments.
 * https://oeis.org/A297044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Count the maximal monotone (strictly increasing or decreasing)
 * runs in the base‑16 representation of n.  Equal consecutive digits form
 * a separate piece.  The function returns the number of such pieces. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* behaviour below the offset is irrelevant */

    /* Extract base‑16 digits (least‑significant first) */
    uint8_t digits[64];            /* 64 digits are far more than enough for uint64_t */
    int k = 0;
    idx_t tmp = n;
    do {
        digits[k++] = (uint8_t)(tmp & 0xFu);
        tmp >>= 4;
    } while (tmp);

    seq_t count = 0;
    int i = k - 1;                 /* start from the most significant digit */

    while (i > 0) {
        uint8_t a = digits[i];
        uint8_t b = digits[i - 1];

        if (a == b) {               /* plateau piece */
            count++;
            while (i > 0 && digits[i] == digits[i - 1]) i--;
        } else {                    /* monotone run (asc or desc) */
            count++;
            int sign = (a < b) ? 1 : -1;   /* a<b -> ascending, else descending */
            i--;                         /* move to the next digit inside the run */
            while (i > 0) {
                uint8_t c = digits[i];
                uint8_t d = digits[i - 1];
                if (c == d) break;       /* equal breaks the run */
                int ns = (c < d) ? 1 : -1;
                if (ns != sign) break;   /* change of direction breaks the run */
                i--;                     /* keep moving inside the run */
            }
        }
    }

    return count;
}

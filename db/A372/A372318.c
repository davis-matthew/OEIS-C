/*
 * A372318  For any n > 0, reverse digits in blocks in balanced ternary
 *          expansion of n where blocks are separated by 1's; a(0) = 0.
 * https://oeis.org/A372318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 6561, u32 n <= 6561, u64 n <= 6561,
 *            u128 n <= 6561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A372318: for every n ≥ 0,
   write n in balanced ternary, reverse each block of digits that lie
   between 1's, and rebuild the number. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Step 1 – convert to balanced ternary (least‑significant digit first). */
    seq_t v = (seq_t)n;            /* work value */
    int8_t digits[128];            /* digits: -1, 0, 1 */
    int len = 0;

    if (v == 0)                    /* a(0) = 0 */
        return 0;

    while (v > 0) {
        int r = (int)(v % 3);      /* r ∈ {0,1,2} */
        v /= 3;
        if (r == 2) {
            r = -1;
            v += 1;               /* carry */
        }
        digits[len++] = (int8_t)r;
        if (len >= 128)           /* safety guard – should never happen */
            break;
    }

    /* Step 2 – process blocks between 1's (most‑significant digit first). */
    int8_t msb[128];
    for (int i = 0; i < len; ++i)
        msb[i] = digits[len - 1 - i];          /* reverse order */

    int8_t block[128], blockLen = 0;
    int8_t out[128], outLen = 0;

    for (int i = 0; i < len; ++i) {
        int8_t d = msb[i];
        if (d == 1) {
            /* flush current block (reversed) */
            for (int j = blockLen - 1; j >= 0; --j)
                out[outLen++] = block[j];
            blockLen = 0;
            /* keep the separating 1 */
            out[outLen++] = 1;
        } else {
            block[blockLen++] = d;
        }
    }
    /* flush final block */
    for (int j = blockLen - 1; j >= 0; --j)
        out[outLen++] = block[j];

    /* Step 3 – convert back to an integer. */
    seq_t result = 0;
    for (int i = 0; i < outLen; ++i) {
        int8_t d = out[i];
        if (d == -1)
            result = result * 3 - 1;
        else
            result = result * 3 + (seq_t)d;   /* d is 0 or 1 */
    }

    return result;
}

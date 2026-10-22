/*
 * A306107  Numbers with digits in {0,1,2,3,4} such that every other digit is
 *          strictly less than its neighbors.
 * https://oeis.org/A306107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: exhaustively scan integers.  
 * Returns the n-th integer whose decimal digits are in {0,1,2,3,4}
 * and for which every even-position digit (counting from 1 at the left)
 * is strictly less than its immediate neighbors.  The sequence starts
 * 0,1,2,3,4,10,20,21,... (as in OEIS A306107).  */
#include <stdbool.h>

/* Helper: check whether a value satisfies the digit rule. */
static bool valid(seq_t x) {
    /* Buffer for decimal digits (up to 64 digits covers 128‑bit integers). */
    uint8_t digits[64];
    int len = 0;
    seq_t t = x;

    /* Extract digits (least significant first). */
    do {
        unsigned d = (unsigned)(t % 10);           /* digit 0..9 */
        if (d > 4) return false;                  /* digit outside allowed set */
        digits[len++] = d;
        t = t / 10;
    } while (t > 0);

    /* Evaluate the neighbors constraint on even positions. */
    for (int i = 0; i < len; ++i) {
        if (((len - i) & 1) == 0) {                /* even position from left */
            if (i > 0 && digits[i] >= digits[i - 1]) return false; /* right neighbor */
            if (i + 1 < len && digits[i] >= digits[i + 1]) return false; /* left neighbor */
        }
    }
    return true;
}

/* Required external function for the sequence. */
seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t cur = 0;
    while (1) {
        if (valid(cur)) {
            ++count;
            if (count == n) {
                return cur;
            }
        }
        ++cur;            /* advance to next integer */
    }
}

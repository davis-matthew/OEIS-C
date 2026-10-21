/*
 * A296713  Numbers whose base-10 digits d(m), d(m-1), ..., d(0) have #(rises)
 *          > #(falls); see Comments.
 * https://oeis.org/A296713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 101, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A296713 – numbers whose decimal digits have more rises than falls.
   The n-th term is returned by searching forward from 1, keeping count of
   numbers that satisfy the condition until the desired index is reached. */
#include <stdint.h>
#include <stdbool.h>

static bool good(seq_t x) {
    /* Count rises and falls in the decimal representation of x. */
    uint8_t dig[128];
    int len = 0;
    while (x != (seq_t)0) {
        dig[len++] = (uint8_t)(x % (seq_t)10);
        x /= (seq_t)10;
    }
    if (len <= 1) return false;   /* zero rises and falls when len == 1 */
    int rises = 0, falls = 0;
    for (int i = len - 1; i > 0; --i) {
        if (dig[i] < dig[i - 1]) ++rises;
        else if (dig[i] > dig[i - 1]) ++falls;
    }
    return rises > falls;
}

seq_t a(idx_t n) {
    /* Return the n-th number (n >= 1) with more rises than falls. */
    seq_t num = (seq_t)1;
    idx_t count = 0;
    while (1) {
        if (good(num)) {
            ++count;
            if (count == n) return num;
        }
        ++num;
    }
}

/*
 * A297031  Number of pieces in the list d(m), d(m-1), ..., d(0) of base-3
 *          digits of n; see Comments.
 * https://oeis.org/A297031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = number of maximal monotonic pieces (ascents, descents, or
 *            plateaus) in the base‑3 representation of n (most significant to
 *            least)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n) {
    /* Handle possible n==0 (offset starts at 1; this case is harmless). */
    if (n == 0) return 0;

    /* Extract base‑3 digits (least significant first).  Maximum 41 digits for 64‑bit n. */
    unsigned char digits[64];
    size_t len = 0;
    idx_t x = n;
    do {
        digits[len++] = (unsigned char)(x % 3);
        x /= 3;
    } while (x);

    /* Convert to most significant first. */
    for (size_t i = 0, j = len - 1; i < j; ++i, --j) {
        unsigned char tmp = digits[i];
        digits[i]   = digits[j];
        digits[j]   = tmp;
    }

    /* A single‑digit n contributes 0 pieces. */
    if (len <= 1) return 0;

    /* Count maximal monotonic runs. */
    seq_t pieces = 1;                      /* first run */
    int prev_diff = (int)digits[1] - (int)digits[0];
    for (size_t i = 2; i < len; ++i) {
        int diff = (int)digits[i] - (int)digits[i - 1];
        if (diff != prev_diff) {
            ++pieces;
            prev_diff = diff;
        }
    }
    return pieces;
}

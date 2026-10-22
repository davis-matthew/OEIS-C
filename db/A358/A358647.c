/*
 * A358647  Final digit reached by traveling right (with wraparound) through
 *          the digits of n. Each move steps right k places, where k is the
 *          digit at the beginning of the move. Moves begin at the most
 *          significant digit and d moves are made, where d is the number of
 *          digits in n.
 * https://oeis.org/A358647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = final digit reached by moving right in the decimal *
 *            representation of n, starting at the most significant digit and
 *            * making exactly d moves, where d is the number of digits in n.
 *            * The step length for each move is the digit at the current
 *            position. * This implementation works for any idx...
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Extract digits of n into an array in most‑significant‑digit order. */
    idx_t tmp = n;
    unsigned d;
    if (tmp == 0) {
        d = 1;
    } else {
        d = 0;
        while (tmp) {
            ++d;
            tmp /= 10;
        }
    }

    uint8_t digits[20]; /* enough for 64‑bit numbers */
    tmp = n;
    if (tmp == 0) {
        digits[0] = 0;
    } else {
        for (int i = (int)d - 1; i >= 0; --i) {
            digits[i] = tmp % 10;
            tmp /= 10;
        }
    }

    /* Simulate the traversal. */
    unsigned pos = 0;
    for (unsigned step_i = 0; step_i < d; ++step_i) {
        unsigned step = digits[pos];
        pos = (pos + step) % d;
    }

    return (seq_t)digits[pos];
}

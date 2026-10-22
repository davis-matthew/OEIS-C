/*
 * A343602  For any positive number n, the balanced ternary representation of
 *          a(n) is obtained by left-rotating the balanced ternary
 *          representation of n until a nonzero digit appears again as the
 *          leftmost digit; a(0) = 0.
 * https://oeis.org/A343602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) is obtained by left‑rotating the balanced ternary
 *            representation of n until a non‑zero digit becomes the leftmost,
 *            after at least one rotation (a(0)=0)
 * Range:     i8 n <= 121, i16 n <= 9841, i32 n <= 9841, i64 n <= 9841,
 *            i128 n <= 9841, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* needed for idx_t and seq_t types */

seq_t a(idx_t n)
{
    /* a(0) = 0 */
    if (n == 0)
        return 0;

    /* Balanced ternary digits of n, least–significant first. */
    int lsb[80];
    int k = 0;
    seq_t x = (seq_t)n;

    /* Convert to balanced ternary. */
    while (x != 0) {
        seq_t r = x % (seq_t)3;
        x = x / (seq_t)3;
        if (r == 2) {            /* map 2 → –1 with a carry */
            r = -1;
            x += 1;
        }
        lsb[k++] = (int)r;
    }

    /* Build msb‑first array from the LSB array. */
    int msb[80];
    for (int i = 0; i < k; ++i)
        msb[i] = lsb[k - 1 - i];

    /* Find the first rotation position (s ≥ 1) where the leftmost
       digit after rotation is non‑zero. */
    int s = 1;
    while (msb[s % k] == 0)
        ++s;

    /* Compute the integer value of the rotated representation. */
    seq_t w = 0;
    for (int i = 0; i < k; ++i) {
        w = w * (seq_t)3 + (seq_t)msb[(s + i) % k];
    }

    return w;
}

/*
 * A343603  For any positive number n, the balanced ternary representation of
 *          a(n) is obtained by right-rotating the balanced ternary
 *          representation of n until a nonzero digit appears again as the
 *          leftmost digit; a(0) = 0.
 * https://oeis.org/A343603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 121, i16 n <= 9841, i32 n <= 9841, i64 n <= 9841,
 *            i128 n <= 9841, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A343603: a(n) is obtained by rotating the balanced ternary representation of n.
   For n>0 the digits are right‑rotated until a non‑zero digit becomes the most
   significant. The algorithm runs in O(log3 n) time. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)        /* a(0) = 0 */
        return 0;

    /* temporary array: least‑significant digit first */
    int8_t tmp[64];
    int tlen = 0;
    idx_t v = n;
    while (v > 0) {
        int r = v % 3;
        v /= 3;
        if (r == 2) {                  /* 2 → -1, carry 1 */
            tmp[tlen++] = -1;
            v += 1;
        } else {                       /* 0 or 1 */
            tmp[tlen++] = (int8_t)r;
        }
    }
    if (tlen == 0) {                   /* safety (n==0 handled above) */
        tmp[tlen++] = 0;
    }

    /* reverse to most‑significant digit first */
    int8_t digits[64];
    for (int i = 0; i < tlen; ++i)
        digits[i] = tmp[tlen - 1 - i];
    int len = tlen;

    /* find the index of the rightmost non‑zero digit */
    int cur = 0;
    for (int i = len - 1; i >= 0; --i) {
        if (digits[i] != 0) {
            cur = i;
            break;
        }
    }

    /* interpret the rotated sequence as a base‑3 number */
    seq_t res = 0;
    for (int step = 0; step < len; ++step) {
        int pos = (cur + step) % len;
        res = res * 3 + (seq_t)digits[pos];
    }
    return res;
}

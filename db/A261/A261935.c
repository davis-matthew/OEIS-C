/*
 * A261935  The first of seventeen consecutive positive integers the sum of
 *          the squares of which is equal to the sum of the squares of two
 *          consecutive positive integers.
 * https://oeis.org/A261935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the first integer of 17 consecutive numbers whose sum of
 *            squares * equals the sum of squares of two consecutive integers.
 *            * The solutions of x² – 34 y² = 815 with y = a+8 yield a(n) = y
 *            − 8. * Two interleaved linear recurrences for the y‐values hold:
 *            * y_{k+1} = 70 y_k − y_{k-1} * with th...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)23;

    if ((n & 1) == 1) {                /* odd n */
        idx_t k = (n - 1) / 2;         /* 0‑based index in odd sequence */
        if (k == 0) return (seq_t)5;
        if (k == 1) return (seq_t)933;
        seq_t y_prev = (seq_t)13;
        seq_t y_curr = (seq_t)941;
        for (idx_t i = 2; i <= k; ++i) {
            seq_t y_next = (seq_t)70 * y_curr - y_prev;
            y_prev = y_curr;
            y_curr = y_next;
        }
        return y_curr - (seq_t)8;
    } else {                           /* even n */
        idx_t k = (n - 2) / 2;         /* 0‑based index in even sequence */
        if (k == 0) return (seq_t)23;
        if (k == 1) return (seq_t)2175;
        seq_t y_prev = (seq_t)31;
        seq_t y_curr = (seq_t)2183;
        for (idx_t i = 2; i <= k; ++i) {
            seq_t y_next = (seq_t)70 * y_curr - y_prev;
            y_prev = y_curr;
            y_curr = y_next;
        }
        return y_curr - (seq_t)8;
    }
}

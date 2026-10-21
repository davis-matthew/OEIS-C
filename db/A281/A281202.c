/*
 * A281202  Number of n X 5 0..1 arrays with no element equal to more than one
 *          of its horizontal and antidiagonal neighbors, with the exception
 *          of exactly one element, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A281202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 26, u64 n <= 61, u128 n <= 133,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A281202: Number of n×5 binary arrays with each element equal to at most one
 * of its horizontal and antidiagonal neighbours, except exactly one element.
 * Recurrence (empirical):  a(n) = 4*a(n-1) – 4*a(n-2) – 2*a(n-4) + 4*a(n-5) – a(n-8)
 * for n ≥ 9.  Initial values:
 *   a(1)=5, a(2)=52, a(3)=176, a(4)=470, a(5)=1141,
 *   a(6)=2602, a(7)=5712, a(8)=12208.
 */
#include <stdint.h>

static const seq_t init[8] = {
    5,      /* a(1) */
    52,     /* a(2) */
    176,    /* a(3) */
    470,    /* a(4) */
    1141,   /* a(5) */
    2602,   /* a(6) */
    5712,   /* a(7) */
    12208   /* a(8) */
};

seq_t a(idx_t n)
{
    if (n <= 8U) {
        return init[n - 1U];
    }

    /* prev[0] = a(n-8), prev[1] = a(n-7), ..., prev[7] = a(n-1) */
    seq_t prev[8];
    for (idx_t i = 0; i < 8U; ++i) {
        prev[i] = init[i];
    }

    seq_t val = 0;
    for (idx_t i = 9U; i <= n; ++i) {
        val = 4 * prev[7] - 4 * prev[6] - 2 * prev[4] + 4 * prev[3] - prev[0];
        /* Rotate the circular buffer: drop a(n-8), shift others, insert a(n) */
        for (idx_t j = 0; j < 7U; ++j) {
            prev[j] = prev[j + 1];
        }
        prev[7] = val;
    }

    return val;
}

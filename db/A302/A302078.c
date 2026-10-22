/*
 * A302078  Number of n X 5 0..1 arrays with every element equal to 0, 1, 3 or
 *          4 horizontally or antidiagonally adjacent elements, with upper
 *          left element zero.
 * https://oeis.org/A302078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) +17*a(n-2) -38*a(n-3) -68*a(n-4) +136*a(n-5)
 *            +64*a(n-6) -128*a(n-7), (empirical recurrence for n > 9) Initial
 *            values (n = 1 .. 9): 8, 35, 154, 692, 3048, 13712, 61536,
 *            277248, 1248512
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, NULL */

/* sequence values are stored in constants that fit in seq_t.
   For any seq_t type the literals are converted implicitly. */
static const seq_t base[9] = {
    (seq_t)8,      (seq_t)35,   (seq_t)154,
    (seq_t)692,    (seq_t)3048, (seq_t)13712,
    (seq_t)61536,  (seq_t)277248, (seq_t)1248512
};

seq_t a(idx_t n)
{
    /* n is 1‑based.  Any n < 1 requests undefined behaviour.
       We simply return 0 for that case. */
    if (n < 1)
        return (seq_t)0;

    if (n <= 9)
        return base[n - 1];

    /* Keep the last seven values needed for the recurrence.
       last7[0] = a(i-7), last7[1] = a(i-6), ..., last7[6] = a(i-1) */
    seq_t last7[7];

    /* initialise last7 with a(3) .. a(9) which are base[2] .. base[8] */
    for (idx_t j = 0; j < 7; ++j)
        last7[j] = base[2 + j];

    /* iterate from i = 10 up to n */
    for (idx_t i = 10; i <= n; ++i) {
        seq_t pos = 3 * last7[6]  /* a(i-1) */
                 + 17 * last7[5] /* a(i-2) */
                 + 136 * last7[2]/* a(i-5) */
                 + 64  * last7[1];/* a(i-6) */

        seq_t neg = 38 * last7[4] /* a(i-3) */
                 + 68 * last7[3] /* a(i-4) */
                 + 128 * last7[0];/* a(i-7) */

        seq_t next = pos - neg;

        /* shift the ring buffer: drop the oldest, keep the newest */
        for (idx_t k = 0; k < 6; ++k)
            last7[k] = last7[k + 1];
        last7[6] = next;
    }

    /* after the loop, last7[6] holds a(n) */
    return last7[6];
}

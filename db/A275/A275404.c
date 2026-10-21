/*
 * A275404  Number of 5 X n 0..2 arrays with no element equal to any value at
 *          offset (-2,-1) (-2,0) or (-1,1) and new values introduced in order
 *          0..2.
 * https://oeis.org/A275404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 12, a(2) = 63, a(3) = 394, a(4) = 2404, a(5) = 14884,
 *            a(6) = 91849, a(7) = 566331, a(8) = 3495079, a(9) = 21564220,
 *            a(10) = 133044851. For n > 10: a(n) = 2*a(n-1) + 15*a(n-2) +
 *            55*a(n-3) + 69*a(n-4) + 8*a(n-5) - 50*a(n-6) + 14*a(n-7) +
 *            a(n-8) - a(n-9)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Return values for the first 10 indices directly. */
    if (n == 0) return (seq_t)0;

    static const seq_t init[10] = {
        (seq_t)12,
        (seq_t)63,
        (seq_t)394,
        (seq_t)2404,
        (seq_t)14884,
        (seq_t)91849,
        (seq_t)566331,
        (seq_t)3495079,
        (seq_t)21564220,
        (seq_t)133044851
    };

    if (n <= 10) {
        return init[n - 1];
    }

    /* Circular buffer holding a(n-9) ... a(n-1).  Initially holds
       a(2) ... a(10) for the first step where n == 11. */
    seq_t last[9];
    for (idx_t i = 0; i < 9; ++i) {
        last[i] = init[i + 1];  /* init[1] == a(2), ..., init[9] == a(10) */
    }

    /* Compute a(n) iteratively for n = 11 .. target n. */
    for (idx_t cur = 11; cur <= n; ++cur) {
        /* Map the recurrence terms from the buffer:
           a1 = a(n-1), a2 = a(n-2), ..., a9 = a(n-9). */
        seq_t a1 = last[8];
        seq_t a2 = last[7];
        seq_t a3 = last[6];
        seq_t a4 = last[5];
        seq_t a5 = last[4];
        seq_t a6 = last[3];
        seq_t a7 = last[2];
        seq_t a8 = last[1];
        seq_t a9 = last[0];

        /* Apply the linear recurrence. */
        seq_t curr = 2 * a1
                    + 15 * a2
                    + 55 * a3
                    + 69 * a4
                    + 8  * a5
                    - 50 * a6
                    + 14 * a7
                    + a8
                    - a9;

        /* Shift the buffer left by one position:
           discard a(n-9) (last[0]) and insert new a(n) at last[8]. */
        for (size_t i = 0; i < 8; ++i) {
            last[i] = last[i + 1];
        }
        last[8] = curr;
    }

    /* After the loop, last[8] holds a(n). */
    return last[8];
}

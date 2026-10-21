/*
 * A260241  Number of (n+2) X (1+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00000001 00000101 or 00001001.
 * https://oeis.org/A260241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 9*a(n-3) - 4*a(n-4) + 9*a(n-5) -
 *            9*a(n-6) for n > 7 a(1) = 48, a(2) = 99, a(3) = 256, a(4) = 635,
 *            a(5) = 1598, a(6) = 4027, a(7) = 10110 (OEIS A260241)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 21, u64 n <= 45, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial values a(1) ... a(7) */
    static const seq_t init[7] = {(seq_t)48, (seq_t)99, (seq_t)256,
                                  (seq_t)635, (seq_t)1598, (seq_t)4027,
                                  (seq_t)10110};

    if (n == 0)
        return (seq_t)0;                 /* outside offset, but defined */

    if (n <= 7)
        return init[n - 1];

    /* prev[0] = a(n-6), prev[1] = a(n-5), ..., prev[5] = a(n-1) */
    seq_t prev[6] = { init[1], init[2], init[3],
                      init[4], init[5], init[6] };   /* start with a2..a7 */

    idx_t i;
    for (i = 8; i <= n; ++i)
    {
        seq_t next = 2 * prev[5]
                   - 2 * prev[4]
                   + 9 * prev[3]
                   - 4 * prev[2]
                   + 9 * prev[1]
                   - 9 * prev[0];

        /* shift window: drop a(n-6), add a(n) */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = prev[5];
        prev[5] = next;
    }

    return prev[5];   /* a(n) */
}

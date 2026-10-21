/*
 * A296968  Number of n X 2 0..1 arrays with each 1 adjacent to 2, 4 or 5
 *          king-move neighboring 1s.
 * https://oeis.org/A296968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 6*a(n-3) - 4*a(n-4) + 2*a(n-5);
 *            Initial terms: a(1)=1, a(2)=5, a(3)=11, a(4)=21, a(5)=59
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 25, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base values for n = 1..5 */
    const seq_t base[5] = {1, 5, 11, 21, 59};

    if (n <= 5)
        return base[n - 1];

    /* Keep the last five terms, indices:
       prev[0] = a(k-5), prev[1] = a(k-4), ..., prev[4] = a(k)   */
    seq_t prev[5];
    prev[0] = base[0]; prev[1] = base[1]; prev[2] = base[2];
    prev[3] = base[3]; prev[4] = base[4];

    for (idx_t i = 6; i <= n; ++i) {
        /* Compute a(i) from the recurrence */
        seq_t next = 2 * prev[4] - prev[3] + 6 * prev[2] - 4 * prev[1] + 2 * prev[0];
        /* Shift the window */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = next;
    }
    return prev[4];  /* a(n) */
}

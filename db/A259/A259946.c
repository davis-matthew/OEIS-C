/*
 * A259946  Number of (n+2) X (2+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00000000 00000001 or 00000101.
 * https://oeis.org/A259946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 18, u64 n <= 39, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for A259946:
   a(n) = 2*a(n-1) + 2*a(n-2) + a(n-3) + 6*a(n-4)
          - 14*a(n-5) - 4*a(n-6) + 2*a(n-7)
          - 4*a(n-8) + 4*a(n-9)   for n > 10
   Initial values (offset 1):
   a(1)=60,  a(2)=170,  a(3)=549,  a(4)=1507,  a(5)=4128,
   a(6)=11933, a(7)=34337, a(8)=97374, a(9)=277073, a(10)=791655
   The sequence grows very fast; for 64‑bit builds only values
   that fit in the target type are checked.  The function is
   O(n) with O(1) memory. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* First 10 terms, 1‑based index */
    static const seq_t init[11] = {
        0,          /* unused */
        60, 170, 549, 1507, 4128, 11933, 34337,
        97374, 277073, 791655
    };

    if (n <= 10)
        return init[(size_t)n];

    /* Buffer holding the last 9 values a[i-9] … a[i-1] */
    seq_t prev[9];
    for (int k = 0; k < 9; ++k)
        prev[k] = init[(size_t)(k + 2)];   /* prev[0] = a[2] … prev[8] = a[10] */

    seq_t next;
    for (idx_t i = 11; i <= n; ++i) {
        /* Compute a(i) using the recurrence and the buffer */
        next = 2 * prev[8] + 2 * prev[7] + prev[6] + 6 * prev[5]
             + 2 * prev[2] + 4 * prev[0];
        /* Subtract the negative terms */
        next -= 14 * prev[4] + 4 * prev[3] + 4 * prev[1];

        /* Shift buffer: drop a(i-9), shift left, insert a(i) at the end */
        for (int k = 0; k < 8; ++k)
            prev[k] = prev[k + 1];
        prev[8] = next;
    }

    return prev[8];   /* a(n) */
}

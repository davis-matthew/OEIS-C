/*
 * A275560  Number of n X 3 0..2 arrays with no element equal to any value at
 *          offset (-2,0) (-1,2) or (0,-2) and new values introduced in order
 *          0..2.
 * https://oeis.org/A275560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 8*a(n-2) - 16*a(n-4) for n>5 Initial values:
 *            a(1)=3, a(2)=36, a(3)=126, a(4)=504, a(5)=1872, a(6)=7200
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* below offset – any value ok */

    /* Return precomputed values for n ≤ 6 */
    if (n <= 6) {
        static const seq_t init[7] = {
            0,             /* dummy for index 0 */
            3,             /* a(1) */
            36,            /* a(2) */
            126,           /* a(3) */
            504,           /* a(4) */
            1872,          /* a(5) */
            7200           /* a(6) */
        };
        return init[n];
    }

    /* Allocate array for dynamic computation */
    seq_t *arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!arr) return 0;                /* allocation failure, value irrelevant */

    /* Initialize known terms */
    arr[0] = 0;
    arr[1] = 3;
    arr[2] = 36;
    arr[3] = 126;
    arr[4] = 504;
    arr[5] = 1872;
    arr[6] = 7200;

    /* Compute remaining terms using the recurrence */
    for (idx_t i = 7; i <= n; ++i) {
        /* a(i) = 2*a(i-1) + 8*a(i-2) - 16*a(i-4) */
        arr[i] = arr[i - 1] * 2
               + arr[i - 2] * 8
               - arr[i - 4] * 16;
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}

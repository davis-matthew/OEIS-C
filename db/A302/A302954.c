/*
 * A302954  Number of 3Xn 0..1 arrays with every element equal to 1, 2, 3, 4
 *          or 5 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A302954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 15*a(n-2) + 37*a(n-3) + 80*a(n-4) + 68*a(n-5)
 *            + 26*a(n-6) + 5*a(n-7) for n>9 Initial values: a(1)=0, a(2)=11,
 *            a(3)=86, a(4)=519, a(5)=3626, a(6)=26167, a(7)=185810,
 *            a(8)=1317541, a(9)=9353154
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle trivial cases directly. */
    if (n == 0) return (seq_t)0;  /* undefined, but safe */

    /* Base values for n = 1..9. Index 0 is unused. */
    static const seq_t base[10] = {
        0,  /* placeholder for index 0 */
        0,  11, 86, 519, 3626,
        26167, 185810, 1317541, 9353154
    };

    if (n <= 9) return base[n];

    /* Allocate an array big enough to hold values up to n. */
    seq_t *arr = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!arr) return (seq_t)0; /* memory allocation failed */

    /* Initialize the first nine terms. */
    for (idx_t i = 1; i <= 9; ++i)
        arr[i] = base[i];

    /* Compute the remaining terms using the recurrence. */
    for (idx_t i = 10; i <= n; ++i)
        arr[i] = 4 * arr[i - 1]
               + 15 * arr[i - 2]
               + 37 * arr[i - 3]
               + 80 * arr[i - 4]
               + 68 * arr[i - 5]
               + 26 * arr[i - 6]
               + 5  * arr[i - 7];

    seq_t result = arr[n];
    free(arr);
    return result;
}

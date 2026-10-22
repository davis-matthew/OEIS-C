/*
 * A304341  Number of nX2 0..1 arrays with every element unequal to 0, 1, 2, 4
 *          or 5 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) + 6*a(n-3) - 10*a(n-4) - 8*a(n-5),
 *            with starting values a(1)=2, a(2)=4, a(3)=14, a(4)=28, a(5)=94,
 *            a(6)=284
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                          /* undefined for n < 1 */

    if (n <= 6) {                                 /* initial values */
        static const seq_t init[] = { 0, 2, 4, 14, 28, 94, 284 };
        return init[n];
    }

    /* Allocate space for terms 1..n. */
    seq_t *const arr = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!arr) return 0;                           /* allocation failed */

    /* Seed the recurrence. */
    arr[1] = 2;
    arr[2] = 4;
    arr[3] = 14;
    arr[4] = 28;
    arr[5] = 94;
    arr[6] = 284;

    /* Compute subsequent terms using the given recurrence. */
    for (idx_t i = 7; i <= n; ++i)
        arr[i] = 2 * arr[i - 1] + 2 * arr[i - 2] +
                  6 * arr[i - 3] - 10 * arr[i - 4] -
                  8 * arr[i - 5];

    seq_t result = arr[n];
    free(arr);
    return result;
}

/*
 * A252534  Number of (2+2) X (n+2) 0..3 arrays with every 3 X 3 subblock row
 *          and diagonal sum equal to 0 2 3 6 or 7 and every 3 X 3 column and
 *          antidiagonal sum not equal to 0 2 3 6 or 7.
 * https://oeis.org/A252534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 15, u32 n <= 39, u64 n <= 87, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Empirical recurrence: a(n) = 6*a(n-3) - 8*a(n-6) for n > 10
 * with initial values
 *   a(1)=595, a(2)=337, a(3)=341, a(4)=462, a(5)=706,
 *   a(6)=1000, a(7)=1468, a(8)=2420, a(9)=3480, a(10)=5240
 */

#include <stdlib.h>

static const seq_t BASE[10] = {
    595, 337, 341, 462, 706, 1000, 1468, 2420, 3480, 5240
};

seq_t a(idx_t n)
{
    /* answer for the first 10 terms */
    if (n <= 10)
        return BASE[n - 1];

    /* allocate array [0..n] – index 0 is unused */
    seq_t *arr = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!arr)  /* allocation failure – not expected in tests */
        return 0;

    /* store the base values */
    for (idx_t i = 1; i <= 10; ++i)
        arr[i] = BASE[i - 1];

    /* use the recurrence for n > 10 */
    for (idx_t i = 11; i <= n; ++i)
        arr[i] = (seq_t)6 * arr[i - 3] - (seq_t)8 * arr[i - 6];

    seq_t result = arr[n];
    free(arr);
    return result;
}

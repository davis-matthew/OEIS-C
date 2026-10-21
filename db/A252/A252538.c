/*
 * A252538  Number of (6+2) X (n+2) 0..3 arrays with every 3 X 3 subblock row
 *          and diagonal sum equal to 0 2 3 6 or 7 and every 3 X 3 column and
 *          antidiagonal sum not equal to 0 2 3 6 or 7.
 * https://oeis.org/A252538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-3) - 32*a(n-6) for n > 8 (Empirical recurrence
 *            from the OEIS entry). Initial terms: a(1)=649, a(2)=964,
 *            a(3)=1748, a(4)=2504, a(5)=7136, a(6)=13184, a(7)=19232,
 *            a(8)=54656
 * Range:     u8 none, u16 n <= 8, u32 n <= 24, u64 n <= 56, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n=0, return 0 */

    /* Initial terms */
    static const seq_t base[8] =
        {649, 964, 1748, 2504, 7136, 13184, 19232, 54656};

    if (n <= 8) return base[n - 1];

    /* Allocate space for all terms up to n */
    seq_t *arr = (seq_t *)malloc((size_t)n * sizeof(seq_t));
    if (!arr) return 0; /* allocation failure */

    /* Populate known initial values */
    for (idx_t i = 1; i <= 8; ++i)
        arr[i - 1] = base[i - 1];

    /* Compute further terms using the recurrence */
    for (idx_t i = 9; i <= n; ++i) {
        /* a(i) = 12*a(i-3) - 32*a(i-6)  */
        arr[i - 1] = (seq_t)12 * arr[i - 4] - (seq_t)32 * arr[i - 7];
    }

    seq_t result = arr[n - 1];
    free(arr);
    return result;
}

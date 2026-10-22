/*
 * A376727  Expansion of (1 + x^3 - x^4)/((1 + x^3 - x^4)^2 - 4*x^3).
 * https://oeis.org/A376727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-3) + 2*a(n-4) - a(n-6) + 2*a(n-7) - a(n-8). Initial
 *            values: a(0)=1, a(1)=0, a(2)=0, a(3)=3, a(4)=1, a(5)=0, a(6)=5,
 *            a(7)=10
 * Range:     u8 n <= 16, u16 n <= 30, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free */

seq_t a(idx_t n)
{
    /* Initial values up to index 7 */
    static const seq_t base[8] = {1, 0, 0, 3, 1, 0, 5, 10};

    /* If index is within the initial segment, return directly */
    if (n < 8)
        return base[(size_t)n];

    /* Allocate an array large enough to hold 0..n */
    size_t sz = (size_t)(n) + 1;
    seq_t *arr = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!arr)  /* allocation failed – return 0 as a safe fallback */
        return (seq_t)0;

    /* Copy initial values */
    for (size_t i = 0; i < 8; ++i)
        arr[i] = base[i];

    /* Compute the remaining terms using the recursion */
    for (size_t i = 8; i < sz; ++i)
    {
        arr[i] = 2 * arr[i - 3] + 2 * arr[i - 4]
                 - arr[i - 6] + 2 * arr[i - 7] - arr[i - 8];
    }

    seq_t result = arr[n];
    free(arr);
    return result;
}

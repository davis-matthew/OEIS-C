/*
 * A266333  G.f. = b(2)*b(4)*b(6)/(x^8+x^6-x^5-x^3-x+1), where b(k) =
 *          (1-x^k)/(1-x).
 * https://oeis.org/A266333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-5) - a(n-6) - a(n-8) (n >= 10)
 *            Initial values: a(0)=1, a(1)=4, a(2)=9, a(3)=17, a(4)=29,
 *            a(5)=47, a(6)=74, a(7)=113, a(8)=170, a(9)=253
 * Range:     u8 n <= 9, u16 n <= 23, u32 n <= 52, u64 n <= 110, u128 n <= 226,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* first ten terms from the series expansion of the generating function */
    static const seq_t init[10] = {1, 4, 9, 17, 29, 47, 74, 113, 170, 253};

    if (n < 10)
        return init[n];

    /* work with size_t for array indexing to avoid overflow issues with idx_t */
    size_t n_s = (size_t)n;
    size_t len  = n_s + 1;

    seq_t *arr = (seq_t *)malloc(len * sizeof(seq_t));
    if (!arr)                 /* allocation failure – return a known value */
        return init[9];

    /* initialise the first ten terms */
    for (size_t i = 0; i <= 9; ++i)
        arr[i] = init[i];

    /* recurrence for n >= 10 :
       a[n] = a[n-1] + a[n-3] + a[n-5] - a[n-6] - a[n-8]  */
    for (size_t i = 10; i <= n_s; ++i) {
        arr[i] = arr[i-1] + arr[i-3] + arr[i-5];
        arr[i] -= arr[i-6] + arr[i-8];
    }

    seq_t result = arr[n_s];
    free(arr);
    return result;
}

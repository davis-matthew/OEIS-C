/*
 * A295854  a(n) = a(n-1) + 3*a(n-2) -2*a(n-3) - 2*a(n-4), where a(0) = -2,
 *          a(1) = -2, a(2) = 2, a(3) = 1.
 * https://oeis.org/A295854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 2*a(n-3) - 2*a(n-4) Initial values:
 *            a(0) = -2, a(1) = -2, a(2) = 2, a(3) = 1
 * Range:     i8 n <= 7, i16 n <= 17, i32 n <= 40, i64 n <= 86, i128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the four known initial values directly. */
    if (n == 0) return (seq_t)(-2);
    if (n == 1) return (seq_t)(-2);
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)1;

    /* Compute the sequence up to the requested index. */
    size_t len = (size_t)n + 1;
    seq_t *arr = NULL;

#ifdef __cplusplus
    arr = new seq_t[len];
#else
    arr = (seq_t *)malloc(len * sizeof(seq_t));
    if (!arr) return (seq_t)0;   /* unlikely on success run, defensive */
#endif

    arr[0] = (seq_t)(-2);
    arr[1] = (seq_t)(-2);
    arr[2] = (seq_t) 2;
    arr[3] = (seq_t) 1;

    for (size_t i = 4; i <= n; ++i) {
        arr[i] = arr[i-1]
               + (seq_t)3 * arr[i-2]
               - (seq_t)2 * arr[i-3]
               - (seq_t)2 * arr[i-4];
    }

    seq_t result = arr[n];

#ifdef __cplusplus
    delete [] arr;
#else
    free(arr);
#endif

    return result;
}

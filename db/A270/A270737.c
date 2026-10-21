/*
 * A270737  a(n) = ((n+2)/2)*Sum_{k=0..n/2} (Sum_{i=0..n-2*k}
 *          (binomial(k+1,n-2*k-i)*binomial(k+i,k))*F(k+1)/(k+1)), where F(k)
 *          is Fibonacci numbers.
 * https://oeis.org/A270737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-5) + a(n-6) Initial values: a(0)=1,
 *            a(1)=3, a(2)=5, a(3)=10, a(4)=20, a(5)=42
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <cstddef>
#else
#include <stddef.h>
#endif

seq_t a(idx_t n) {
    /* Handle the trivial cases directly */
    if (n == 0) return 1;
    if (n == 1) return 3;
    if (n == 2) return 5;
    if (n == 3) return 10;
    if (n == 4) return 20;
    if (n == 5) return 42;

    /* Allocate an array to hold the values up to n */
    size_t len = (size_t)n + 1;
#ifdef __cplusplus
    seq_t *arr = new seq_t[len];
#else
    seq_t *arr = (seq_t *)calloc(len, sizeof(seq_t));
#endif

    /* Establish base cases */
    arr[0] = 1;
    arr[1] = 3;
    arr[2] = 5;
    arr[3] = 10;
    arr[4] = 20;
    arr[5] = 42;

    /* Iterate using the recurrence */
    for (size_t i = 6; i < len; ++i) {
        arr[i] = (2 * arr[i - 1]) + (2 * arr[i - 5]) + arr[i - 6];
    }

    /* Copy the result before freeing the temporary array */
    seq_t result = arr[n];

#ifdef __cplusplus
    delete [] arr;
#else
    free(arr);
#endif
    return result;
}

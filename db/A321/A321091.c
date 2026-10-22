/*
 * A321091  Continued fraction expansion of the constant z that satisfies:
 *          CF(3*z, n) = CF(z, n) + 10, for n >= 0, where CF(z, n) denotes the
 *          n-th partial denominator in the continued fraction expansion of z.
 * https://oeis.org/A321091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   Computation of OEIS A321091 using the recurrence rules provided
 *            in the OEIS entry. The function calculates a(0..n) iteratively,
 *            which works for both the C and C++ builds
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External function requested by the test harness */
seq_t a(idx_t n)
{
    /* Allocate an array of size n+1.  In the C++ build seq_t is a
       class type, so we use new[]; in the C build it is a plain
       integer type and we use malloc. */
#ifdef __cplusplus
    seq_t *arr = new seq_t[(size_t)n + 1];
#else
    seq_t *arr = (seq_t *)malloc(((size_t)n + 1) * sizeof(seq_t));
#endif

    if (arr == NULL) abort();  /* allocation failure */

    /* term a(0) = 4 */
    arr[0] = 4;

    /* Compute terms from 1 to n */
    for (idx_t i = 1; i <= n; ++i) {
        idx_t mod3 = i % 3;
        if (mod3 == 0) {
            /* a(3*n) = 3 for n >= 1 */
            arr[i] = 3;
        } else if (mod3 == 1) {
            /* a(3*n+1) follows special rules depending on i % 9 */
            idx_t mod9 = i % 9;
            if (mod9 == 1) {
                arr[i] = 1;                            /* a(9*n+1) = 1 */
            } else if (mod9 == 4) {
                idx_t m = (i - 4) / 9;                 /* i = 9*m+4 */
                /* a(9*n+4) = 3 - a(3*n+1);  with n = m */
                arr[i] = 3 - arr[3 * m + 1];
            } else { /* mod9 == 7 */
                arr[i] = 2;                            /* a(9*n+7) = 2 */
            }
        } else { /* mod3 == 2  ->  a(3*n+2) = 3 - a(3*n+1) */
            arr[i] = 3 - arr[i - 1];
        }
    }

    seq_t result = arr[n];

    /* Free the allocated memory */
#ifdef __cplusplus
    delete[] arr;
#else
    free(arr);
#endif

    return result;
}

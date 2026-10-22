/*
 * A339374  Number of partitions of n into an odd number of triangular
 *          numbers.
 * https://oeis.org/A339374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A007294(n) - A292519(n)) / 2 i.e. number of partitions
 *            of n into an odd number of triangular numbers. We compute this
 *            by a standard coin‑change DP that keeps track of the parity of
 *            the number of parts using two arrays
 * Range:     u8 n <= 52, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* allocate arrays of size n+1, one for even and one for odd
       number of parts (both inclusive of 0 parts). */
#ifdef __cplusplus
    /* In C++ use new[] so constructors/destructors run correctly. */
    seq_t *even = new seq_t[n + 1]();  /* value‑initialised to 0 */
    seq_t *odd  = new seq_t[n + 1]();
    if (even == nullptr || odd == nullptr) {
        return 0;                    /* unlikely, but safe */
    }
#else
    /* In C use malloc and manually initialise to 0. */
    seq_t *even = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *odd  = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!even || !odd) {
        free(even);
        free(odd);
        return 0;
    }
    for (idx_t i = 0; i <= n; ++i) {
        even[i] = 0;
        odd[i]  = 0;
    }
#endif

    /* empty partition uses 0 parts (even parity) */
    even[0] = 1;
    odd[0]  = 0;

    /* process triangular numbers t_k = k*(k+1)/2 in increasing order,
       allowing unlimited use of each triangle (unbounded coin change). */
    for (idx_t k = 1;; ++k) {
        idx_t t = (k * (k + 1)) / 2;
        if (t > n)
            break;
        for (idx_t s = t; s <= n; ++s) {
            /* adding a triangle switches the parity of the partition */
            odd[s]  += even[s - t];
            even[s] += odd[s - t];
        }
    }

    seq_t result = odd[n];

    /* free memory */
#ifdef __cplusplus
    delete[] even;
    delete[] odd;
#else
    free(even);
    free(odd);
#endif

    return result;
}

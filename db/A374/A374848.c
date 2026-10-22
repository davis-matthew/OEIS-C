/*
 * A374848  Obverse convolution A000045**A000045; see Comments.
 * https://oeis.org/A374848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{k=0}^{n} (F(k) + F(n-k)) * where F is the Fibonacci
 *            sequence F(0)=0, F(1)=1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
#include <new>
#endif

seq_t a(idx_t n)
{
    /* Allocate an array to hold Fibonacci numbers up to F(n). */
#ifdef __cplusplus
    seq_t* fib = new seq_t[n + 1];
#else
    seq_t* fib = (seq_t*)malloc((n + 1) * sizeof(seq_t));
#endif
    if (!fib) return 0; /* out of memory */

    /* Compute Fibonacci numbers. */
    fib[0] = 0;
    if (n >= 1u)
        fib[1] = 1;
    for (idx_t i = 2; i <= n; ++i)
        fib[i] = fib[i - 1] + fib[i - 2];

    /* Compute the obverse convolution product. */
    seq_t prod = 1;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t val = fib[k] + fib[n - k];
        prod *= val;
    }

    /* Free the allocated array. */
#ifdef __cplusplus
    delete[] fib;
#else
    free(fib);
#endif

    return prod;
}

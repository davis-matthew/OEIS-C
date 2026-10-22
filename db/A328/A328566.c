/*
 * A328566  a(n) is the sum of the elements of the set O_n = {(n-k) OR k, k =
 *          0..n} (where OR denotes the bitwise OR operator).
 * https://oeis.org/A328566
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the sum of all distinct values of (n-k) OR k, k=0..n.
 *            For a negative index n < -1, a(n)=0. This implementation keeps a
 *            boolean array of size n+1 to deduplicate the OR results
 * Range:     u8 n <= 35, u16 n <= 1097, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < -1)   /* behaviour for indices below the offset not required */
        return 0;

    if (n == -1)  /* explicit base case */
        return 0;

    size_t N = (size_t)n;
    /* Allocate a zero‑initialized boolean array of size N+1.
       The cast is required for C++ compilation. */
    unsigned char *seen =
        (unsigned char *)calloc(N + 1, sizeof(unsigned char));
    if (!seen)
        return 0;   /* allocation failure – unlikely for N ≤ 10⁴ */

    for (size_t k = 0; k <= N; ++k) {
        size_t val = ((size_t)n - k) | k;  /* (n-k) OR k */
        /* val is guaranteed to be ≤ n, so it fits in the array. */
        seen[val] = 1;
    }

    seq_t sum = 0;
    for (size_t i = 0; i <= N; ++i)
        if (seen[i])
            sum += (seq_t)i;

    free(seen);
    return sum;
}

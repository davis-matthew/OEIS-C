/*
 * A328564  a(n) is the sum of the elements of the set A_n = {(n-k) AND k, k =
 *          0..n} (where AND denotes the bitwise AND operator).
 * https://oeis.org/A328564
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = sum of all distinct values of (n-k) & k for k = 0..n,
 *            with offset -1 (a(-1)=0)
 * Range:     u8 n <= 73, u16 n <= 2217, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* a(-1) = 0, defined for all n >= -1 */
    if (n < 0)
        return 0;

    /* Allocate a bitmap for values up to n (inclusive). */
    size_t size = (size_t)n + 1;                     /* n may be 0 */
    bool *seen = (bool *)calloc(size, sizeof(bool));
    if (!seen)                                       /* out-of-memory guard */
        return 0;

    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
    {
        /* Compute the AND value; result fits in [0, n]. */
        seq_t v = ((seq_t)(n - k) & (seq_t)k);

        /* Convert to an index for the bitmap. */
        idx_t iv = (idx_t)v;                         /* safe since v <= n */
        size_t idx = (size_t)iv;

        if (!seen[idx])                              /* first occurrence */
        {
            seen[idx] = true;
            sum += v;
        }
    }

    free(seen);
    return sum;
}

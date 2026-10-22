/*
 * A328565  a(n) is the sum of the elements of the set X_n = {(n-k) XOR k, k =
 *          0..n} (where XOR denotes the bitwise XOR operator).
 * https://oeis.org/A328565
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 1171, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = sum of the distinct values of (n-k) XOR k for k = 0..n.
   For n < 0 the function returns 0 (a(-1) = 0).  The implementation
   uses a small lookup table of size n+1 to filter duplicates. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 0)
        return 0;                      /* a(-1) = 0 */

    /* size_t must be able to hold n+1; n is small (≤ ~10000) */
    size_t size = (size_t)n + 1;
    unsigned char *seen = (unsigned char *)calloc(size, 1);   /* zeroed */

    if (!seen)
        return 0;                      /* allocation failure – return 0 */

    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t val = ((seq_t)(n - k)) ^ ((seq_t)k);
        size_t idx = (size_t)val;      /* val ≤ n, so idx < size */
        if (!seen[idx]) {
            seen[idx] = 1;
            sum += val;
        }
    }

    free(seen);
    return sum;
}

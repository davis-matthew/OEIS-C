/*
 * A368822  Number of compositions of n into two non-refactorable parts.
 * https://oeis.org/A368822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n-1} c(k) * c(n-k), where * c(n) = 1 if n is
 *            NOT a refactorable number, 0 otherwise. * A number m is
 *            refactorable if the number of its positive divisors d(m) *
 *            divides m. Thus c(n) = 1 iff (n % d(n)) != 0
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are supplied by the build system and are integer types.
 * No assumption about which exact type is used.  The code below uses only
 * arithmetic and indexing that are valid for all unsigned integer types.
 */

seq_t a(idx_t n) {
    /* Static cache of divisor counts and c(n) values */
    static idx_t *divcnt = NULL;
    static seq_t *c      = NULL;
    static idx_t  max_precomputed = 0;

    /* Extend the cache if necessary */
    if (n > max_precomputed) {
        idx_t newN = n;
        /* Reallocate arrays to accommodate up to newN (index 0 unused) */
        divcnt = (idx_t *)realloc(divcnt, (newN + 1) * sizeof(idx_t));
        c      = (seq_t *)realloc(c,      (newN + 1) * sizeof(seq_t));

        /* Compute divisor counts for 1..newN */
        for (idx_t i = 1; i <= newN; ++i) divcnt[i] = 0;
        for (idx_t i = 1; i <= newN; ++i) {
            for (idx_t j = i; j <= newN; j += i)
                divcnt[j] += 1;
        }

        /* Compute c(i) = 1 if i is NOT refactorable, else 0 */
        for (idx_t i = 1; i <= newN; ++i) {
            c[i] = ((i % divcnt[i]) != 0) ? 1 : 0;
        }

        max_precomputed = newN;
    }

    /* Compute the convolution sum for a(n) */
    seq_t sum = 0;
    for (idx_t k = 1; k < n; ++k) {
        /* Only 0 or 1 values in c, multiplication is inexpensive */
        sum += c[k] * c[n - k];
    }
    return sum;
}

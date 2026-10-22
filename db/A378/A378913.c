/*
 * A378913  a(n) = sum of all positive m such that sigma(m) divides n, where
 *          sigma is the sum-of-divisors function (A000203).
 * https://oeis.org/A378913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of all positive m such that sigma(m) divides n, where
 *            sigma is the sum-of-divisors function
 * Range:     u8 n <= 71, u16 n <= 2879, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* allocate array of sigma values, indices 0..n (0 unused) */
    size_t N = (size_t)n;
    seq_t *sigma = (seq_t*)calloc(N + 1, sizeof(seq_t));
    if (!sigma) return 0; /* fallback, should not happen in tests */

    /* compute sigma(i) for all i <= n */
    for (size_t i = 1; i <= N; ++i) {
        for (size_t j = i; j <= N; j += i) {
            sigma[j] += (seq_t)i;
        }
    }

    seq_t result = 0;
    for (size_t m = 1; m <= N; ++m) {
        idx_t sm = (idx_t)sigma[m];
        if (sm != 0 && n % sm == 0) {
            result += (seq_t)m;
        }
    }

    free(sigma);
    return result;
}

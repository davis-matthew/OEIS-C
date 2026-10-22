/*
 * A339368  Number of partitions of n into an even number of cubes.
 * https://oeis.org/A339368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of partitions of n into an even number of cubes. a(n) =
 *            (A003108(n) + A292560(n)) / 2 (generating function:
 *            0.5*(∏_{k≥1}1/(1−x^{k^3}) + ∏_{k≥1}1/(1+x^{k^3}))
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <vector>
#endif


seq_t a(idx_t n)
{
    if (n == 0) return 1;

    size_t N = (size_t)n;

#ifdef __cplusplus
    std::vector<seq_t> even(N + 1), odd(N + 1);
    /* vector value‑initialises its elements – zero for fundamental types,
       default constructor for class types.  */
    even[0] = 1;
#else
    seq_t *even = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *odd  = (seq_t*)calloc(N + 1, sizeof(seq_t));
    if (!even || !odd) {
        free(even); free(odd);
        return 0;
    }
    even[0] = 1;
#endif

    for (idx_t k = 1;; ++k) {
        idx_t c = k * k * k;
        if (c > n) break;

#ifdef __cplusplus
        for (size_t s = c; s <= N; ++s) {
            seq_t old_even = even[s];
            seq_t old_odd  = odd[s];
            even[s] = old_even + odd[s - c];
            odd[s]  = old_odd  + even[s - c];
        }
#else
        for (size_t s = c; s <= N; ++s) {
            seq_t old_even = even[s];
            seq_t old_odd  = odd[s];
            even[s] = old_even + odd[s - c];
            odd[s]  = old_odd  + even[s - c];
        }
#endif
    }

    seq_t result = even[N];

#ifndef __cplusplus
    free(even);
    free(odd);
#endif

    return result;
}

/*
 * A339375  Number of partitions of n into an even number of distinct
 *          triangular numbers.
 * https://oeis.org/A339375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = number of partitions of n into an even number of
 *            distinct triangular numbers. * This equals the coefficient of
 *            x^n in * ½ (∏_{k≥1}(1+x^(k(k+1)/2)) + ∏_{k≥1}(1−x^(k(k+1)/2)))
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* Use std::vector for C++: elements are default‑constructed to zero. */
    std::vector<seq_t> even(n + 1), odd(n + 1);
    even[0] = 1;
#else
    /* In C allocate raw memory and initialise elements to zero. */
    seq_t *even = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    seq_t *odd  = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!even || !odd) return 0;              /* out‑of‑memory protection */
    for (idx_t i = 0; i <= n; ++i) even[i] = odd[i] = 0;
    even[0] = 1;
#endif

    for (idx_t k = 1; ; ++k) {
        idx_t t = (k * (k + 1)) / 2;          /* triangular number */
        if (t > n) break;

#ifndef __cplusplus
        /* For unsigned idx_t the loop must terminate before wrapping. */
        for (idx_t s = n; s >= t; --s) {
            seq_t de = even[s] + odd[s - t];
            seq_t dof = odd[s] + even[s - t];
            even[s] = de;
            odd[s]  = dof;
            if (s == t) break;
        }
#else
        for (idx_t s = n; s >= t; --s) {
            seq_t de = even[s] + odd[s - t];
            seq_t dof = odd[s] + even[s - t];
            even[s] = de;
            odd[s]  = dof;
            if (s == t) break;
        }
#endif
    }

#ifdef __cplusplus
    return even[n];
#else
    seq_t result = even[n];
    free(even);
    free(odd);
    return result;
#endif
}

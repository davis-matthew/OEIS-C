/*
 * A374963  a(n) = Sum_{k=1..n-1} sigma(k)*sigma_3(n-k).
 * https://oeis.org/A374963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n-1} sigma(k)*sigma_3(n-k)
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 137, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    idx_t N = n;

#ifndef __cplusplus
    /* C: use malloc, then free */
    seq_t *sigma1 = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *sigma3 = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!sigma1 || !sigma3) {
        free(sigma1);
        free(sigma3);
        return 0;   /* OOM guard */
    }
#else
    /* C++: use std::vector to construct objects properly */
    std::vector<seq_t> sigma1_vec(N + 1), sigma3_vec(N + 1);
    seq_t *sigma1 = sigma1_vec.data();
    seq_t *sigma3 = sigma3_vec.data();
#endif

    /* initialise to zero */
    for (idx_t i = 0; i <= N; ++i) {
        sigma1[i] = 0;
        sigma3[i] = 0;
    }

    /* sieve to compute sigma_1 and sigma_3 up to N */
    for (idx_t i = 1; i <= N; ++i) {
        seq_t i_seq = (seq_t)i;
        seq_t i_sq  = i_seq * i_seq;  /* i^2  */
        seq_t i_cu  = i_sq * i_seq;   /* i^3  */

        for (idx_t j = i; j <= N; j += i) {
            sigma1[j] += i_seq;
            sigma3[j] += i_cu;
        }
    }

    /* convolution sum */
    seq_t acc = 0;
    for (idx_t k = 1; k < n; ++k) {
        acc += sigma1[k] * sigma3[n - k];
    }

#ifndef __cplusplus
    free(sigma1);
    free(sigma3);
#endif

    return acc;
}

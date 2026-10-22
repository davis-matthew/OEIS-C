/*
 * A353288  a(n) = Sum_{k=0..floor(n/2)} k^(n-2*k) * Stirling2(n-k,k).
 * https://oeis.org/A353288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} k^(n-2*k) * Stirling2(n-k,k)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 15, u64 n <= 22, u128 n <= 24,
 *            big any n
 * Helpers:   pow_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    size_t kmax = (size_t)(n / 2);
    size_t nsize = (size_t)n;

    /* Allocation of helper arrays */
#ifdef __cplusplus
    seq_t *prev = new seq_t[kmax + 1];
    seq_t *cur  = new seq_t[kmax + 1];
    seq_t *need = new seq_t[kmax + 1];
    for (size_t i = 0; i <= kmax; ++i) {
        prev[i] = 0;
        cur[i]  = 0;
        need[i] = 0;
    }
#else
    seq_t *prev = (seq_t *)calloc(kmax + 1, sizeof(seq_t));
    seq_t *cur  = (seq_t *)calloc(kmax + 1, sizeof(seq_t));
    seq_t *need = (seq_t *)calloc(kmax + 1, sizeof(seq_t));
#endif

    /* Base row: S(0,0) = 1 */
    prev[0] = (seq_t)1;

    /* Compute Stirling numbers of the second kind and capture needed values */
    for (size_t r = 1; r <= nsize; ++r) {
        cur[0] = 0;
        size_t km = r < kmax ? r : kmax;
        for (size_t k = 1; k <= km; ++k) {
            cur[k] = prev[k] * (seq_t)k + prev[k - 1];
        }

        size_t k_needed = nsize - r;
        if (k_needed <= km) {
            need[k_needed] = cur[k_needed];
        }

        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    /* Sum the terms */
    seq_t sum = 0;
    for (size_t k = 0; k <= kmax; ++k) {
        seq_t pow_val = pow_pow((seq_t)k, nsize - 2 * k);
        sum += pow_val * need[k];
    }

    /* Free the allocated arrays */
#ifdef __cplusplus
    delete[] prev;
    delete[] cur;
    delete[] need;
#else
    free(prev);
    free(cur);
    free(need);
#endif

    return sum;
}

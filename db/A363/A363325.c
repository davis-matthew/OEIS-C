/*
 * A363325  Total number of parts coprime to n in the partitions of n into 7
 *          parts.
 * https://oeis.org/A363325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A363325:
 * a(n) = total number of parts coprime to n over all partitions of n into 7
 * positive parts.  The algorithm enumerates partitions in nondecreasing
 * order using dynamic programming; it works for any seq_t/idx_t pair and
 * compiles under both C11 and C++17. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd_u64.h"

/* Euclidean algorithm for 64‑bit integers.  Works in C and C++. */

seq_t a(idx_t n)
{
    /* Zero for n < 7 because no positive partition into 7 parts exists. */
    if (n < 7u)
        return (seq_t)0;

    /* Convert to size_t for array indexing; n is at most 2^64‑1 and fits. */
    size_t N = (size_t)n;
    size_t stride = N + 1;                /* one row per possible sum */

    /* Allocate DP tables for counts and weighted sums of coprime parts. */
    seq_t *cnt = (seq_t *)calloc((7 + 1) * stride, sizeof(seq_t));
    seq_t *sum = (seq_t *)calloc((7 + 1) * stride, sizeof(seq_t));
    if (!cnt || !sum) {
        free(cnt);
        free(sum);
        return (seq_t)0;                  /* allocation failure – unlikely */
    }

    /* Seed: one way to have 0 parts sum to 0. */
    cnt[0] = (seq_t)1;
    sum[0] = (seq_t)0;

    /* Pre‑compute coprimality flags for each part value 1..n. */
    uint8_t *coprime = (uint8_t *)malloc((N + 1) * sizeof(uint8_t));
    if (!coprime) {
        free(cnt);
        free(sum);
        return (seq_t)0;
    }
    for (size_t v = 1; v <= N; ++v)
        coprime[v] = (gcd_u64(n, v) == 1) ? 1 : 0;

    /* Dynamic programming over part values.  The loops guarantee
     * nondecreasing part sequences, i.e., unordered partitions. */
    for (size_t v = 1; v <= N; ++v) {
        bool flag = coprime[v] != 0;         /* true if v coprime to n */
        for (size_t k = 1; k <= 7; ++k) {
            size_t base_k_minus = (k - 1) * stride;
            size_t base_k       = k * stride;
            for (size_t s = v; s <= N; ++s) {
                seq_t prev_cnt = cnt[base_k_minus + (s - v)];
                if (!prev_cnt)
                    continue;

                /* Increment partition count. */
                cnt[base_k + s] += prev_cnt;

                /* Increment weighted sum of coprime parts. */
                seq_t prev_sum = sum[base_k_minus + (s - v)];
                if (flag)
                    sum[base_k + s] += prev_sum + prev_cnt;
                else
                    sum[base_k + s] += prev_sum;
            }
        }
    }

    seq_t result = sum[7 * stride + N];

    /* Free temporary storage. */
    free(cnt);
    free(sum);
    free(coprime);

    return result;
}

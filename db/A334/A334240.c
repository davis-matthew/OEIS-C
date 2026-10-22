/*
 * A334240  a(n) = exp(-n) * Sum_{k>=0} (k + 1)^n * n^k / k!.
 * https://oeis.org/A334240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = exp(-n) * Σ_{k≥0} (k + 1)^n * n^k / k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   alloc_seq_array, ensure_capacity, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"
#include "../utils/ensure_capacity.h"

static seq_t compute_a_single(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t *S    = alloc_seq_array((size_t)n + 1);
    seq_t *powN = alloc_seq_array((size_t)n + 1);

    /* Initialize Stirling numbers S(0,0)=1, others 0. */
    S[0] = (seq_t)1;
    for (idx_t j = 1; j <= n; ++j) {
        S[j] = (seq_t)0;
    }

    /* Pre‑compute powers of n: powN[j] = n^j. */
    seq_t N = (seq_t)n;
    powN[0] = (seq_t)1;
    for (idx_t j = 1; j <= n; ++j) {
        powN[j] = powN[j - 1] * N;
    }

    seq_t ans   = (seq_t)0;
    seq_t binom = (seq_t)1;

    for (idx_t k = 0; k <= n; ++k) {
        /* Compute B_k(n) = Σ_{j=0..k} S[j] * n^j. */
        seq_t B = (seq_t)0;
        for (idx_t j = 0; j <= k; ++j) {
            B += S[j] * powN[j];
        }

        ans += binom * B;

        if (k < n) {
            /* Update Stirling numbers to next row. */
            for (idx_t j = k + 1; j > 0; --j) {
                S[j] = (seq_t)j * S[j] + S[j - 1];
            }
            S[0] = (seq_t)0;

            /* Update binomial coefficient: C(n,k+1) = C(n,k) * (n-k)/(k+1). */
            binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);
        }
    }

    free_seq_array(S);
    free_seq_array(powN);
    return ans;
}

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;

    if (n >= cache_cap) {
        ensure_capacity((size_t)n + 1, &cache_cap, (void **)&cache, sizeof(seq_t));
    }

    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        cache[i] = compute_a_single((idx_t)i);
    }
    cache_size = (size_t)n + 1;

    return cache[n];
}

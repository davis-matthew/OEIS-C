/*
 * A354324  Expansion of e.g.f. exp( x/8 * (exp(4 * x) - 1) ).
 * https://oeis.org/A354324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2) · Σ_{k=2..n} k · 4^(k-2) · binomial(n−1,k-1) ·
 *            a(n−k), initial values: a(0) = 1, a(1) = 0 This implementation
 *            uses a memoised dynamic‑programming approach that runs in O(n²)
 *            arithmetic operations and works for any non‑negative index n. It
 *            allocates the cache lazily and grows it as ne...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;
    static bool *computed    = NULL;
    static size_t cap        = 0;

    /* Ensure the cache is large enough to hold indices 0..n */
    if ((idx_t)cap <= n) {
        size_t new_cap = (cap == 0) ? 2 : cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;

        seq_t *tmp_seq = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp_seq) return 0;   /* allocation failure → return 0 */
        bool *tmp_bool = (bool *)realloc(computed, new_cap * sizeof(bool));
        if (!tmp_bool) return 0;

        /* Initialise the newly allocated portions */
        for (size_t i = cap; i < new_cap; ++i) {
            tmp_seq[i]  = 0;
            tmp_bool[i] = false;
        }

        cache    = tmp_seq;
        computed = tmp_bool;
        cap      = new_cap;
    }

    /* Compute values up to n that are not yet known */
    for (idx_t i = 0; i <= n; ++i) {
        if (computed[(size_t)i]) continue;

        if (i == 0) {
            cache[0] = 1;
        } else if (i == 1) {
            cache[1] = 0;
        } else {
            seq_t sum   = 0;
            seq_t binom = (seq_t)(i - 1);   /* C(i-1,1) */
            seq_t pow4  = 1;                /* 4^(k-2) for k=2 */

            for (idx_t k = 2; k <= i; ++k) {
                seq_t coef = ((seq_t)k * pow4) / 2;      /* integer division */
                seq_t term = coef * binom;
                term = term * cache[(size_t)(i - k)];
                sum += term;

                if (k < i) {
                    /* Update binom: C(N,j+1) = C(N,j) * (N-j) / (j+1)
                       where N = i-1 and j = k-1 */
                    idx_t j = k - 1;
                    binom = binom * (seq_t)(i - 1 - j) / (seq_t)(j + 1);
                    pow4 *= 4;
                }
            }
            cache[(size_t)i] = sum;
        }
        computed[(size_t)i] = true;
    }

    return cache[(size_t)n];
}

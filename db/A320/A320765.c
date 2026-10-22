/*
 * A320765  Number of ordered set partitions of [n] where the maximal block
 *          size equals nine.
 * https://oeis.org/A320765
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) equals the number of ordered set partitions of [n] * whose
 *            maximal block size is exactly 9. * This is given by the e.g.f. *
 *            1/(1-Sum_{i=1..9} x^i/i!) - 1/(1-Sum_{i=1..8} x^i/i!) * and can
 *            be computed by the recurrence * * A_k(0) = 1 * A_k(n) =
 *            Σ_{i=1}^{min(n,k)} C(n,i) * A_k(n-i) * * with a(...
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 16, u64 n <= 22, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the binomial coefficient C(n,i) for i ≤ 9 using
   multiplicative formula (exact divisions). */
static seq_t binom_small(idx_t n, idx_t k)
{
    seq_t res = 1;
    for (idx_t t = 1; t <= k; ++t) {
        res = res * (seq_t)(n - k + t);
        res = res / (seq_t)t;   /* exact division */
    }
    return res;
}

/* cached values for the two auxiliary sequences */
static seq_t *A8 = NULL;
static seq_t *A9 = NULL;
static idx_t max_computed = 0;   /* highest n computed so far */
static size_t cache_capacity = 0;/* current allocated size */

/* Ensure the cache can hold values up to n. */
static void ensure_capacity(idx_t n)
{
    if (cache_capacity > (size_t)n) return;

    size_t new_cap = (size_t)n + 1;
    seq_t *tmp8 = (seq_t *)realloc(A8, new_cap * sizeof(seq_t));
    seq_t *tmp9 = (seq_t *)realloc(A9, new_cap * sizeof(seq_t));
    if (!tmp8 || !tmp9) abort();     /* allocation failure */

    A8 = tmp8;
    A9 = tmp9;

    /* Initialise newly allocated slots to 0. */
    for (size_t i = cache_capacity; i < new_cap; ++i) {
        A8[i] = 0;
        A9[i] = 0;
    }
    cache_capacity = new_cap;
}

/* Compute A8[i] and A9[i] for all i ≤ n. */
static void compute_up_to(idx_t n)
{
    if (max_computed >= n) return;

    ensure_capacity(n);

    /* Initialize base case if this is the first computation. */
    if (max_computed == 0) {
        A8[0] = 1;
        A9[0] = 1;
    }

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t v8 = 0;
        seq_t v9 = 0;
        idx_t maxj8 = i;
        if (maxj8 > 8) maxj8 = 8;
        for (idx_t j = 1; j <= maxj8; ++j) {
            seq_t b = binom_small(i, j);
            v8 += b * A8[i - j];
        }

        idx_t maxj9 = i;
        if (maxj9 > 9) maxj9 = 9;
        for (idx_t j = 1; j <= maxj9; ++j) {
            seq_t b = binom_small(i, j);
            v9 += b * A9[i - j];
        }

        A8[i] = v8;
        A9[i] = v9;
    }
    max_computed = n;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n < 9) return 0;          /* offset is 9; behaviour below 9 is irrelevant */
    compute_up_to(n);
    return A9[n] - A8[n];
}

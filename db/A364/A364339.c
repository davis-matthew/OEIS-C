/*
 * A364339  G.f. satisfies A(x) = (1 + x) * (1 + x*A(x)^6).
 * https://oeis.org/A364339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..n} binomial(6*k+1,k) * binomial(6*k+1,n-k) /
 *            (6*k+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   binom, to_size_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0)=1, a(1)=2, ... as defined in OEIS A364339 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binom.h"
#include "../utils/to_size_t.h"

static seq_t *seq_cache   = NULL;   /* a(0)..a(cache_cap-1) */
static seq_t *bin_cache   = NULL;   /* binom(6*k+1,k) for k < bin_cap   */
static size_t cache_cap   = 0;
static size_t bin_cap     = 0;
static size_t cache_size  = 0;   /* number of values computed (== cache_cap after grow) */
static size_t bin_size    = 0;   /* number of binomials stored     */

/* Grow an array so that it can hold at least 'need+1' elements */
static void ensure_cache(size_t need)
{
    if (need + 1 <= cache_cap)
        return;
    size_t new_cap = (need + 1) * 2;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) exit(EXIT_FAILURE);
    if (seq_cache) {
        for (size_t i = 0; i < cache_size; ++i)
            new_arr[i] = seq_cache[i];
        free(seq_cache);
    }
    seq_cache = new_arr;
    cache_cap = new_cap;
}

/* Grow the binom cache to store at least 'k+1' entries */
static void ensure_bin(size_t k)
{
    if (k + 1 <= bin_cap)
        return;
    size_t new_cap = (k + 1) * 2;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) exit(EXIT_FAILURE);
    if (bin_cache) {
        for (size_t i = 0; i < bin_size; ++i)
            new_arr[i] = bin_cache[i];
        free(bin_cache);
    }
    bin_cache = new_arr;
    bin_cap   = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Ensure cache can hold element n */
    ensure_cache(to_size_t(n));

    /* Compute any missing values up to n */
    for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
        seq_t sum = 0;

        /* k ranges from 0 to i */
        for (idx_t k = 0; k <= i; ++k) {
            /* Build binom(6*k+1,k) if necessary */
            ensure_bin(to_size_t(k));
            if ((size_t)k >= bin_size) {
                seq_t val = binom((idx_t)(6ULL * k + 1), (idx_t)k);
                bin_cache[(size_t)k] = val;
                bin_size = to_size_t(k) + 1;
            }

            seq_t bin1 = bin_cache[(size_t)k];
            seq_t bin2 = binom((idx_t)(6ULL * k + 1), (idx_t)(i - k));
            seq_t divisor = (seq_t)(6ULL * k + 1);

            /* (bin1 / divisor) * bin2  -- exact division */
            seq_t t1 = bin1 / divisor;
            seq_t term = t1 * bin2;
            sum += term;
        }
        seq_cache[(size_t)i] = sum;
    }
    cache_size = (size_t)n + 1;
    return seq_cache[(size_t)n];
}

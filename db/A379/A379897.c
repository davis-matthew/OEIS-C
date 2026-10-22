/*
 * A379897  E.g.f. A(x) satisfies A(x) = 1/(exp(-x*A(x)^3) - x).
 * https://oeis.org/A379897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..n} (n+2*k+1)^(k-1) *
 *            binomial(n+2*k+1,n-k)/k!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow, binomial, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"
#include "../utils/seq_realloc.h"

/* ---------- factorial cache ---------- */
static seq_t *fact_cache = NULL;
static size_t   fact_cap   = 0;
static idx_t    fact_len   = 0;

/* Ensure fact_cache has room for factorials up to `upto` (inclusive). */
static void ensure_factorials(idx_t upto)
{
    if (upto < fact_len) return;
    if (fact_cap < (size_t)(upto + 1)) {
        seq_t *old = fact_cache;
        fact_cache = seq_realloc(old, fact_cap, (size_t)(upto + 1));
        fact_cap   = (size_t)(upto + 1);
    }
    if (fact_len == 0) {
        fact_cache[0] = (seq_t)1;
        fact_len      = 1;
    }
    for (idx_t i = fact_len; i <= upto; ++i) {
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
    }
    fact_len = upto + 1;
}

/* ---------- sequence cache ---------- */
static seq_t *cache      = NULL;
static size_t   cache_cap = 0;
static idx_t    cache_len = 0;

/* Ensure cache has room for indices up to `upto` (inclusive). */
static void ensure_cache_space(idx_t upto)
{
    if (upto < cache_cap) return;
    seq_t *old = cache;
    cache = seq_realloc(old, cache_cap, (size_t)(upto + 1));
    cache_cap = (size_t)(upto + 1);
}

/* ---------- main function ---------- */
seq_t a(idx_t n)
{
    if (n < cache_len) return cache[n];

    ensure_cache_space(n);
    for (idx_t i = cache_len; i <= n; ++i) {
        ensure_factorials(i);
        seq_t sum = fact_cache[i];          /* k = 0 contributes n! */

        for (idx_t k = 1; k <= i; ++k) {
            idx_t base_idx = i + 2 * k + 1;
            seq_t pow_val  = big_pow((seq_t)base_idx, (seq_t)(k - 1));
            seq_t bin_val  = binomial(base_idx, i - k);
            seq_t factor   = fact_cache[i] / fact_cache[k]; /* n! / k! */
            seq_t term     = pow_val * bin_val;
            term = term * factor;
            sum += term;
        }
        cache[i] = sum;
    }
    cache_len = n + 1;
    return cache[n];
}

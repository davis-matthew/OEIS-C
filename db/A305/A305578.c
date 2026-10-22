/*
 * A305578  a(n) = Sum_{k=0..n} binomial(n,k)*k!!*(n - k)!!.
 * https://oeis.org/A305578
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0..n} binomial(n,k) * k!! * (n-k)!!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 27, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Static cache for double–factorials.  df_cache[i] holds i!!.
   The cache grows only as far as needed. */
static seq_t *df_cache = NULL;
static size_t df_cap   = 0;

/* Ensure df_cache contains values 0!! … n!!.  If the cache is too
   small it is reallocated and all entries up to n are computed. */
static void
ensure_double_factorial(idx_t n)
{
    if (df_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_arr) return;        /* out of memory – leave old cache */
        new_arr[0] = (seq_t)1;
        if (n >= 1) new_arr[1] = (seq_t)1;
        for (idx_t i = 2; i <= n; ++i)
            new_arr[i] = (seq_t)i * new_arr[i - 2];
        free(df_cache);
        df_cache = new_arr;
        df_cap   = new_cap;
    }
}

/* External, non‑static function required by the build system. */
seq_t a(idx_t n)
{
    ensure_double_factorial(n);

    seq_t sum   = (seq_t)0;
    seq_t binom = (seq_t)1;   /* C(n,0) */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = binom;
        term *= df_cache[k];
        term *= df_cache[n - k];
        sum += term;

        if (k < n) {
            /* binom ← binom * (n-k) / (k+1) – exact division */
            binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);
        }
    }
    return sum;
}

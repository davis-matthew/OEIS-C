/*
 * A368575  a(n) = n! * Sum_{k=0..n} binomial(k+3,4) / k!.
 * https://oeis.org/A368575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0; a(n) = n*a(n-1) + binomial(n+3,4) for n≥1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   binom_n_plus3_4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n! * Σ_{k=0..n} binomial(k+3,4)/k!  */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binom_n_plus3_4.h"

static seq_t *seq_cache = NULL; /* array of computed terms */
static size_t cache_cap   = 0;  /* capacity of seq_cache     */
static size_t cache_len   = 0;  /* number of terms already computed (indices 0..cache_len-1) */

/* ensure seq_cache has room for indices 0..n   (i.e. at least n+1 elements) */
static void ensure_cache(idx_t n)
{
    if (cache_cap > (size_t)n) return;           /* already enough */

    size_t new_cap = cache_cap > 0 ? cache_cap : 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                            /* allocation failed */

    seq_cache = tmp;
    cache_cap = new_cap;
}

/* public function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    ensure_cache(n);          /* need room for indices 0..n */

    if (cache_len == 0) {      /* initialise first term */
        seq_cache[0] = 0;
        cache_len = 1;
    }

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev  = seq_cache[i - 1];
        seq_t mult  = (seq_t)i * prev;
        seq_t add   = binom_n_plus3_4(i);
        seq_cache[i] = mult + add;
    }
    cache_len = (size_t)n + 1;   /* we now have computed up to n */

    return seq_cache[n];
}

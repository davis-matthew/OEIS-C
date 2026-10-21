/*
 * A260616  Irregular triangle read by rows: continued fraction expansion of
 *          k/n, 1 <= k <= n.
 * https://oeis.org/A260616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cofr, tabf
 * Formula:   a(n) = continued‑fraction terms of k/n for 1 ≤ k ≤ n, flattened
 *            row‑wise. The function lazily generates the terms as requested
 * Range:     u8 n <= 9176, u16 n <= 9176, u32 n <= 9176, u64 n <= 9176,
 *            u128 n <= 9176, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;          /* memoised terms */
static size_t cache_cap = 0;         /* capacity of the cache */
static idx_t cache_len = 0;          /* number of terms stored */
static idx_t next_row = 1;           /* next row to generate */

/* Append a term to the cache, reallocating if necessary. */
static void push_term(seq_t term)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_t *new_ptr = seq_realloc(cache, cache_cap, new_cap);
        if (!new_ptr) abort();          /* allocation failed */
        cache      = new_ptr;
        cache_cap  = new_cap;
    }
    cache[cache_len++] = term;
}

/* Ensure that at least 'need' terms are in the cache. */
static void generate_terms(idx_t need)
{
    while (cache_len < need) {
        idx_t row = next_row++;
        for (idx_t k = 1; k <= row; ++k) {
            idx_t a = k;
            idx_t b = row;
            while (1) {
                idx_t q = a / b;           /* quotient */
                push_term((seq_t)q);        /* store as seq_t */
                idx_t r = a % b;           /* remainder */
                if (r == 0) break;
                a = b;
                b = r;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, return 0 for safety */
    generate_terms(n);
    return cache[n - 1];
}

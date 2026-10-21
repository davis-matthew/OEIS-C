/*
 * A271990  g_n(9) where g is the weak Goodstein function defined in A266202.
 * https://oeis.org/A271990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * a(n) = weak Goodstein iteration of starting value 9.
 * For n ≥ 0:
 *   a(0) = 9
 *   a(n+1) = ((int base = 2 + n) interpret a(n) in base,
 *             construct the same digits in base+1,
 *             evaluate, then subtract 1)
 *
 * This implementation memoises the sequence so that calls are
 * O(1) on already computed indices.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoised values of a(n) */
static idx_t cache_len = 0;   /* number of stored values (indices 0..cache_len-1) */

/* Ensure cache contains values up to index n. */
static void ensure_cache(idx_t n)
{
    if (n < cache_len) return;   /* already available */

    idx_t new_len = n + 1;
    seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (!new_cache) return;      /* allocation failure – caller will get garbage */

    /* copy previous values if any */
    if (cache) {
        for (idx_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;

    /* initialise first term if not yet present */
    if (cache_len == 0) {
        cache[0] = 9;
        cache_len = 1;
    }

    /* compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* i >= 1 because cache_len==1 at first */
        seq_t v = cache[i - 1];

        idx_t base_idx = 2 + (i - 1);   /* base for this step */
        seq_t base_seq     = (seq_t)base_idx;
        seq_t base_plus    = (seq_t)(base_idx + 1);

        seq_t pow  = 1;
        seq_t next = 0;

        while (v > 0) {
            seq_t digit = v % base_seq;
            next += digit * pow;
            pow  *= base_plus;
            v    /= base_seq;
        }
        next -= 1;                  /* subtract 1 as in weak Goodstein */
        cache[i] = next;
    }

    cache_len = n + 1;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}

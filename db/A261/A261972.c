/*
 * A261972  The first of three consecutive positive integers the sum of the
 *          squares of which is equal to the sum of the squares of four
 *          consecutive positive integers.
 * https://oeis.org/A261972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n‑1) – 15*a(n‑2) + a(n‑3) for n>3 with a(1)=25,
 *            a(2)=361, a(3)=5041
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cache[i] holds a(i+1) */
static size_t cache_cap = 0;         /* allocated capacity */
static size_t cache_len = 0;         /* number of terms already computed */

/* Make sure the cache can hold at least n terms (n >= 1). */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n;
    if (need <= cache_cap)
        return;

    seq_t *new_cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
    if (!new_cache)                   /* allocation failure – abort */
        abort();

    /* Zero any newly allocated entries. */
    for (size_t i = cache_cap; i < need; ++i)
        new_cache[i] = (seq_t)0;

    cache = new_cache;
    cache_cap = need;
}

/* Computes the n-th term of sequence A261972. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;              /* undefined for n < 1 */

    ensure_cache(n);

    /* Initialise the first three terms if they are not yet set. */
    if (cache_len < 3) {
        if (n >= 1) cache[0] = (seq_t)25;
        if (n >= 2) cache[1] = (seq_t)361;
        if (n >= 3) cache[2] = (seq_t)5041;
        cache_len = (n >= 3 ? 3 : (size_t)n);
    }

    /* Compute any missing terms up to n. */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        seq_t t1 = (seq_t)15 * cache[i - 1];
        seq_t t2 = (seq_t)15 * cache[i - 2];
        cache[i] = t1 - t2 + cache[i - 3];
    }

    cache_len = (size_t)n;
    return cache[(size_t)n - 1];
}

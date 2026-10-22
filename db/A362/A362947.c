/*
 * A362947  a(0) = 0, a(1) = 0; for n > 1, a(n) is the number of pairs of
 *          consecutive terms whose product has same value as a(n-2) * a(n-1).
 * https://oeis.org/A362947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 0. For n > 1, a(n) = number of previous
 *            consecutive pairs (a(i), a(i+1)) whose product equals a(n-2) *
 *            a(n-1). The sequence is calculated recursively: each a(n)
 *            depends only on earlier terms and the count of prior pairs with
 *            a specific product
 * Range:     u8 n <= 1309, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache for memoisation */
static seq_t *cache      = NULL;  /* array of computed terms          */
static size_t cache_len  = 0;     /* number of terms stored (cache_len == N+1 if a(0)..a(N) are computed) */
static size_t cache_cap  = 0;     /* allocated capacity */

/* Ensure the cache has room for at least `need` elements.
   `need` is declared as a size_t to keep the arithmetic sane. */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 4;
    while (new_cap < need) new_cap <<= 1;          /* grow exponentially */
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();                         /* out of memory  */
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Compute terms up to index `n` (inclusive) using the definition.
   The function is idempotent; if the cache already contains all
   needed terms it returns immediately. */
static void build_up_to(idx_t n)
{
    /* Nothing to do if already built enough. */
    if ((idx_t)cache_len > n) return;

    /* Allocate/expand cache to hold up to n+1 terms. */
    ensure_cache_capacity((size_t)(n + 1));

    /* Initialise the first two terms if they are not yet set. */
    if (cache_len == 0) {
        cache[0] = 0;
        cache_len = 1;
    }
    if (cache_len == 1) {
        cache[1] = 0;
        cache_len = 2;
    }

    /* Build terms sequentially. */
    while ((idx_t)cache_len <= n) {
        idx_t i = cache_len;                     /* index of term to compute (i >= 2) */
        seq_t prod = cache[i - 2] * cache[i - 1]; /* a(i-2) * a(i-1) */

        /* Count previous adjacent pairs with the same product. */
        seq_t count = 0;
        for (idx_t j = 0; j <= i - 2; ++j) {     /* j runs over 0 .. i-2   */
            if (cache[j] * cache[j + 1] == prod) {
                ++count;
            }
        }
        cache[i] = count;
        ++cache_len;
    }
}

/* Public function required by the OEIS format. */
seq_t a(idx_t n)
{
    build_up_to(n);
    return cache[n];
}

/*
 * A266504  a(n) = 2*a(n - 2) + a(n - 4) with a(0) = a(1) = 2, a(2) = 1, a(3)
 *          = 3.
 * https://oeis.org/A266504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 26, u32 n <= 50, u64 n <= 100,
 *            u128 n <= 202, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence:
   a(n) = 2*a(n-2) + a(n-4)
   a(0) = 2, a(1) = 2, a(2) = 1, a(3) = 3 */
/* The function memoizes computed values to support calls in any order. */

#include <stddef.h>
#include <stdlib.h>

/* External types supplied by the build environment. */
extern seq_t a(idx_t n);

static seq_t *cache = NULL;          /* array of computed values    */
static size_t cache_cap = 0;         /* allocated capacity in elements   */
static idx_t   max_comp = 0;         /* highest index currently filled */

/* Reallocate cache array to fit at least (n+1) elements. */
static void
grow_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap) return;          /* already enough */

    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while ((size_t)n + 1 > new_cap) new_cap <<= 1;   /* give room for n */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return;                               /* abort on out‑of‑memory */
    /* Zero the newly allocated part to avoid garbage. */
    for (size_t i = cache_cap; i < new_cap; ++i) tmp[i] = 0;
    cache = tmp;
    cache_cap = new_cap;
}

/* Set the four initial terms whenever the array is large enough. */
static void
set_bases(void)
{
    if (cache_cap > 0) cache[0] = 2;
    if (cache_cap > 1) cache[1] = 2;
    if (cache_cap > 2) cache[2] = 1;
    if (cache_cap > 3) cache[3] = 3;
}

/* Memoized recurrence implementation. */
seq_t a(idx_t n)
{
    grow_cache(n);
    set_bases();

    /* If we have already computed up to n, just return the value. */
    if (max_comp >= n) return cache[n];

    /* Compute values for indices greater than max_comp up to n. */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        if (i <= 3) continue;                     /* bases already set */
        cache[i] = (seq_t)2 * cache[i - 2] + cache[i - 4];
    }

    max_comp = n >= 3 ? n : (idx_t)3;              /* at most 3 in case of tiny n */
    return cache[n];
}

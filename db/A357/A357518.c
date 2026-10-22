/*
 * A357518  Unique fixed point of the two-block substitution 00->111, 01->110,
 *          10->101, 11->100.
 * https://oeis.org/A357518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 0 For n ≥ 3: a(3n) = 1 - a(2n) a(3n+1) = 1
 *            a(3n+2) = 1 - a(2n+1) This is equivalent to a(n) = A087088(n)
 *            mod 2. The implementation caches computed values up to the
 *            largest requested index, expanding the array as needed. It works
 *            for any integer type seq_t (unsigned integer or arb...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */
#include <stddef.h>   /* size_t */

/* static storage for memoization */
static seq_t *cache   = NULL;   /* array of a(i) values, 0‑based index */
static size_t  cache_cap = 0;   /* allocated size of cache */
static idx_t   computed = 0;    /* highest index computed so far (0 means none) */

/* ensure the cache can hold at least n elements (1‑based) */
static void
ensure_capacity(idx_t n)
{
    if ((size_t)n <= cache_cap) return;          // already large enough

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while ((size_t)n > new_cap) new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                           /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* compute values up to n (inclusive) */
static void
compute_up_to(idx_t n)
{
    if (n <= computed) return;                  /* nothing to do */

    ensure_capacity(n);

    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = 1;
        } else if (i == 2) {
            val = 0;
        } else {
            idx_t rem = i % 3;
            if (rem == 1) {
                val = 1;
            } else if (rem == 0) {               /* i = 3k  */
                idx_t k   = i / 3;
                idx_t idx = (k << 1) - 1;        /* (2k) – 1  */
                val = 1 - cache[idx];
            } else {                              /* rem == 2, i = 3k+2, i>=5  */
                idx_t k   = (i - 2) / 3;
                idx_t idx = (k << 1) + 1;         /* (2k+1)  */
                val = 1 - cache[idx - 1];
            }
        }
        cache[i - 1] = val;                     /* store result */
    }
    computed = n;
}

/* public function required by the build system */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, return 0 */
    compute_up_to(n);
    return cache[n - 1];
}

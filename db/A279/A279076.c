/*
 * A279076  Maximum starting value of X such that repeated replacement of X
 *          with X-ceiling(X/6) requires n steps to reach 0.
 * https://oeis.org/A279076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(a(n-1)*6/5) + 1 a(0) = 0, a(1) = 1, a(2) = 2, a(3)
 *            = 3, a(4) = 4, a(5) = 5
 * Range:     u8 n <= 24, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function memoizes the sequence, allowing arbitrary call order. */
#include <stdlib.h>

/* static cache for already computed terms */
static seq_t *cache = NULL;   /* array of seq_t values */
static idx_t cache_len = 0;   /* allocated length of the cache */
static idx_t max_done = 0;    /* highest index whose value is computed */

/* ensure that the cache can hold index `n` */
static void ensure_capacity(idx_t n)
{
    if (cache_len > n)
        return;
    size_t old_len = (size_t)cache_len;
    size_t new_len = (cache_len == 0) ? 16 : (size_t)cache_len * 2;
    while (new_len <= (size_t)n)
        new_len *= 2;
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    cache_len = (idx_t)new_len;
    for (size_t i = old_len; i < new_len; ++i)
        cache[i] = (seq_t)0;
}

/* initialise cache for the first call */
static void init_cache(void)
{
    if (cache != NULL)
        return;
    ensure_capacity(5);               /* guarantee indices 0..5 */
    for (idx_t i = 0; i <= 5; ++i)
        cache[i] = (seq_t)i;          /* a(0)=0..a(5)=5 */
    max_done = 5;
}

/* public function */
seq_t a(idx_t n)
{
    init_cache();                     /* set up first six terms */
    if (n <= max_done)                /* already computed */
        return cache[n];
    ensure_capacity(n);
    /* compute all missing values up to n */
    for (idx_t i = max_done + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t next = (prev * (seq_t)6) / (seq_t)5 + (seq_t)1;
        cache[i] = next;
    }
    max_done = n;
    return cache[n];
}

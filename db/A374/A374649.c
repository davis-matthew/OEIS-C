/*
 * A374649  a(n) = n! - 2(n-1)! + 3(n-2)!.
 * https://oeis.org/A374649
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! - 2*(n-1)! + 3*(n-2)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation caches factorials to avoid recomputation. */
#include <stdlib.h>

/* Cached factorial array. */
static seq_t *fact_cache = NULL;   /* factorial values 0! … n!. */
static idx_t  fact_max  = 0;       /* highest n for which cache is filled. */

static void ensure_factorial(idx_t n)
{
    /* Grow cache if necessary. */
    if (n <= fact_max)
        return;

    idx_t new_max = n;
    size_t new_cap = (size_t)new_max + 1;  /* array size = new_max+1 */
    if (fact_cache == NULL) {
        fact_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    } else {
        fact_cache = (seq_t*)realloc(fact_cache, new_cap * sizeof(seq_t));
    }
    /* Initialize new entries. */
    if (fact_max == 0) {
        fact_cache[0] = (seq_t)1;   /* 0! = 1 */
    }
    idx_t i;
    for (i = fact_max + (fact_max ? 1 : 1); i <= new_max; ++i) {
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
    }
    fact_max = new_max;
}

/* Compute a(n) = n! - 2*(n-1)! + 3*(n-2)!; valid for n >= 2. */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;   /* behaviour below offset irrelevant */

    ensure_factorial(n);           /* guarantees fact_cache[0…n] exist */

    seq_t fn  = fact_cache[n];     /* n! */
    seq_t fn1 = fact_cache[n - 1]; /* (n-1)! */
    seq_t fn2 = fact_cache[n - 2]; /* (n-2)! */

    seq_t res = fn - (seq_t)2 * fn1 + (seq_t)3 * fn2;
    return res;
}

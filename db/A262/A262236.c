/*
 * A262236  Number of (n+3) X (1+3) 0..1 arrays with each row and column
 *          divisible by 11, read as a binary number with top and left being
 *          the most significant bits.
 * https://oeis.org/A262236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-5) + 3*a(n-6) - 2*a(n-7),
 *            a(1)=2, a(2)=3, a(3)=6, a(4)=12, a(5)=24, a(6)=47, a(7)=94
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;          /* Cache of computed terms. */
static size_t cache_cap = 0;         /* Allocated size of cache. */
static idx_t max_computed = 0;       /* Largest index that has been computed. */

/* Ensure cache can store indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;   /* indices 0..n inclusive.          */
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap > 0 ? cache_cap * 2 : 8;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the base values of the sequence. */
static void init_base(void)
{
    if (cache != NULL) return;
    ensure_capacity(7);
    cache[1] = (seq_t)2;
    cache[2] = (seq_t)3;
    cache[3] = (seq_t)6;
    cache[4] = (seq_t)12;
    cache[5] = (seq_t)24;
    cache[6] = (seq_t)47;
    cache[7] = (seq_t)94;
    max_computed = 7;
}

/* Compute the sequence up to n (inclusive). */
static void compute_to(idx_t n)
{
    if (n <= max_computed) return;
    ensure_capacity(n);
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* Indices 1..7 already set; compute only for i >= 8. */
        cache[i] = 3 * cache[i-1] - 2 * cache[i-2] - cache[i-5]
                 + 3 * cache[i-6] - 2 * cache[i-7];
    }
    max_computed = n;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* Undefined for n < 1. */
    init_base();
    if (n > max_computed) {
        compute_to(n);
    }
    return cache[n];
}

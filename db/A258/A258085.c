/*
 * A258085  Strictly increasing list of F and F - 1, where F = A000045, the
 *          Fibonacci numbers.
 * https://oeis.org/A258085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) – a(n-6) (n > 8)
 * Range:     u8 n <= 22, u16 n <= 44, u32 n <= 90, u64 n <= 182,
 *            u128 n <= 368, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values (1 ≤ n ≤ 8):                          */
/*  a(1)=0, a(2)=1, a(3)=2, a(4)=3, a(5)=4, a(6)=5,    */
/*  a(7)=7, a(8)=8                                    */

#include <stddef.h>
#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* cache of computed terms */
static size_t cache_cap  = 0;      /* current capacity of seq_cache */
static idx_t  cache_max  = 0;      /* highest n for which a(n) is stored */

/* Ensure that seq_cache can hold at least ‘need’ entries. */
static void ensure_cache_cap(idx_t need)
{
    if (cache_cap >= (size_t)need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < (size_t)need) new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Initialise cache with the first eight terms of the sequence. */
static void init_cache(void)
{
    if (seq_cache != NULL) return;
    cache_cap = 8;
    seq_cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
    seq_cache[0] = 0;
    seq_cache[1] = 1;
    seq_cache[2] = 2;
    seq_cache[3] = 3;
    seq_cache[4] = 4;
    seq_cache[5] = 5;
    seq_cache[6] = 7;
    seq_cache[7] = 8;
    cache_max = 8;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;         /* behaviour below offset is irrelevant */
    init_cache();

    if (n <= cache_max) return seq_cache[n-1];

    ensure_cache_cap(n);

    for (idx_t k = cache_max + 1; k <= n; ++k)
    {
        /* For k ≥ 9, use the recurrence:
           a(k) = 2*a(k-2) – a(k-6) = a(k-2) + (a(k-2) – a(k-6))    */
        seq_t t   = seq_cache[k-3];          /* a(k-2)             */
        seq_t diff = t - seq_cache[k-7];     /* a(k-2) – a(k-6)   */
        seq_cache[k-1] = t + diff;           /* a(k)               */
    }

    cache_max = n;
    return seq_cache[n-1];
}

/*
 * A259636  Number of (n+2) X (2+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00000000 or 00000001.
 * https://oeis.org/A259636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 5*a(n-3) + a(n-4) - 3*a(n-5) - 3*a(n-6)
 *            for n>7 with a(1)=29, a(2)=64, a(3)=171, a(4)=365, a(5)=813,
 *            a(6)=1964, a(7)=4499 (OEIS A259636)
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 23, u64 n <= 50, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *seq_cache   = NULL;   /* cache array, indices 0..max_computed */
static idx_t  cache_cap   = 0;      /* allocated size-1 (max index that fits) */
static idx_t  max_computed = 0;     /* highest index already filled in cache */
static bool   cache_init   = false; /* whether base values have been stored */

/* base values at indices 1..7, index 0 unused */
static const seq_t base_vals[8] = {
    (seq_t)0,   /* unused */
    (seq_t)29,  /* a(1) */
    (seq_t)64,  /* a(2) */
    (seq_t)171, /* a(3) */
    (seq_t)365, /* a(4) */
    (seq_t)813, /* a(5) */
    (seq_t)1964,/* a(6) */
    (seq_t)4499 /* a(7) */
};

/* Ensure the cache can hold indices up to n (inclusive) */
static void ensure_capacity(idx_t n)
{
    if (n <= cache_cap)          /* already large enough */
        return;

    /* Allocate new space for indices 0..n  */
    size_t new_size = (size_t)(n + 1);            /* number of elements */
    seq_t *new_cache = (seq_t *)realloc(seq_cache, new_size * sizeof(seq_t));
    if (!new_cache)
        return;              /* allocation failure – caller will handle */

    seq_cache = new_cache;
    cache_cap = n;
}

/* Store the four initial terms that the recurrence needs */
static void init_cache(void)
{
    if (cache_init)
        return;

    ensure_capacity(7);                 /* need indices 0..7 */
    for (idx_t i = 1; i <= 7; ++i)
        seq_cache[i] = base_vals[i];
    max_computed = 7;
    cache_init = true;
}

/* Compute the sequence up to index n using the recurrence */
static void compute_up_to(idx_t n)
{
    if (n <= max_computed)
        return;

    /* n > max_computed. All indices up to max_computed are valid. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t t1 = seq_cache[i - 1];
        seq_t t2 = seq_cache[i - 2];
        seq_t t3 = seq_cache[i - 3];
        seq_t t4 = seq_cache[i - 4];
        seq_t t5 = seq_cache[i - 5];
        seq_t t6 = seq_cache[i - 6];
        seq_cache[i] = t1 + t2 + 5 * t3 + t4 - 3 * t5 - 3 * t6;
    }
    max_computed = n;
}

/* Main public function required by the build system */
seq_t a(idx_t n)
{
    if (n < 1)                /* undefined for n < 1, return 0 */
        return (seq_t)0;

    init_cache();              /* ensures base values are present */

    if (n <= 7)                /* already known */
        return seq_cache[n];

    ensure_capacity(n);        /* allocate if necessary */
    compute_up_to(n);          /* extend the cache */
    return seq_cache[n];
}

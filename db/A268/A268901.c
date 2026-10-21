/*
 * A268901  Number of n X 5 0..2 arrays with some element plus some
 *          horizontally or antidiagonally adjacent neighbor totalling two
 *          exactly once.
 * https://oeis.org/A268901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 30*a(n-1) - 261*a(n-2) + 540*a(n-3) - 324*a(n-4) with
 *            a(1)=96, a(2)=2664, a(3)=54936, a(4)=1004184 (OEIS A268901) The
 *            implementation uses an eager memoisation cache that grows to the
 *            requested index on demand. All arithmetic is performed in the
 *            seq_t type; constants are cast to seq_t to av...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache    = NULL;   /* 1‑based cache; cache[0] unused */
static size_t cache_cap = 0;     /* current allocated capacity (>=0) */
static idx_t  cached_max = 0;    /* highest index stored in cache */

/* Ensure that indices 0..n are writable.  The bump size of 64 keeps
   realloc calls fast for small sequences. */
static void
ensure_cache_cap(idx_t n)
{
    size_t need = (size_t)n + 1;          /* indices 0..n inclusive */
    if (need <= cache_cap) return;

    size_t new_cap = need;
    if (new_cap < 64) new_cap = 64;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the first four terms and set cache[0] to a dummy value. */
static void
init_start(idx_t)
{
    if (cached_max >= 4) return;          /* already done */

    ensure_cache_cap(4);
    cache[0] = (seq_t)0;                  /* unused, but keeps indices 1‑based */
    cache[1] = (seq_t)96;
    cache[2] = (seq_t)2664;
    cache[3] = (seq_t)54936;
    cache[4] = (seq_t)1004184;
    cached_max = 4;
}

/* Extend the cache up to index n using the linear recurrence. */
static void
extend_to(idx_t n)
{
    if (cached_max >= n) return;

    /* Guarantee sufficient capacity before writing new values. */
    ensure_cache_cap(n);

    if (cached_max < 4) init_start(n);    /* set base values if not yet done */

    for (idx_t i = cached_max + 1; i <= n; ++i) {
        seq_t a1 = cache[i-1];
        seq_t a2 = cache[i-2];
        seq_t a3 = cache[i-3];
        seq_t a4 = cache[i-4];

        seq_t val = (seq_t)30  * a1;
        val -= (seq_t)261  * a2;
        val += (seq_t)540  * a3;
        val -= (seq_t)324  * a4;

        cache[i] = val;
    }
    cached_max = n;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* a(0) is undefined; return 0 for safety */
    extend_to(n);
    return cache[n];
}

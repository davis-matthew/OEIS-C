/*
 * A311993  Coordination sequence Gal.5.98.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + a(n-9) - a(n-10) for n > 10,
 * initial values:
 *  a(0)=1, a(1)=4, a(2)=8, a(3)=13, a(4)=17, a(5)=23, a(6)=27,
 *  a(7)=32, a(8)=36, a(9)=40, a(10)=44
 */

#include <stdlib.h>

static const seq_t init_vals[11] = {
    (seq_t)1,  (seq_t)4,  (seq_t)8,  (seq_t)13, (seq_t)17,
    (seq_t)23, (seq_t)27, (seq_t)32, (seq_t)36, (seq_t)40, (seq_t)44
};

/* static cache for memoised values */
static seq_t *cache   = NULL;   /* array of terms */
static idx_t  cap     = 0;      /* allocated capacity */
static idx_t  size    = 0;      /* number of initialized values */

/* ensure that the cache can hold at least 'need' entries (0‑based) */
static void
ensure_capacity(idx_t need)
{
    if (need < cap)
        return;
    idx_t new_cap = (cap == 0) ? 128 : cap * 2;
    while (new_cap <= need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();            /* out of memory */
    cache = tmp;
    cap   = new_cap;
}

/* initialise cache with the first 11 terms if necessary */
static void
init_cache_if_needed(void)
{
    if (cache != NULL)
        return;             /* already initialised */

    /* Allocate at least 11 elements, but we allocate a bit more for future growth */
    cap   = 128;
    cache = (seq_t *)calloc(cap, sizeof(seq_t));
    if (!cache)
        abort();            /* out of memory */
    for (idx_t i = 0; i < 11; ++i)
        cache[i] = init_vals[i];
    size = 11;             /* indices 0 … 10 are valid */
}

/*--- public interface -----------------------------------------------------*/
seq_t
a(idx_t n)
{
    init_cache_if_needed();
    if (n < size)
        return cache[n];

    /* Ensure we have enough space for all new terms up to n */
    ensure_capacity(n);

    /* Compute missing values from the current size up to n */
    for (idx_t i = size; i <= n; ++i) {
        /* recurrence: a(i) = a(i-1) + a(i-9) - a(i-10)   (i > 10) */
        cache[i] = cache[i - 1] + cache[i - 9] - cache[i - 10];
    }
    size = n + 1;

    return cache[n];
}

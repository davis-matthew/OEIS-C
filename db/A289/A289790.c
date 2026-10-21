/*
 * A289790  p-INVERT of A016789, where p(S) = 1 - S - S^2.
 * https://oeis.org/A289790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 5*a(n-2) + 8*a(n-3) + a(n-4) Initial values:
 *            a(0)=2, a(1)=13, a(2)=72, a(3)=385
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if defined(__cplusplus)
#include <cstdlib>
#else
#include <stdlib.h>
#endif

/* Persistent cache for computed terms. */
static seq_t *cache = NULL;        /* array of terms */
static size_t cache_len = 0;       /* number of elements stored */
static size_t cache_cap = 0;       /* allocated capacity */

static void ensure_capacity(size_t new_len)
{
    if (new_len <= cache_cap)
        return;                       /* already enough */

    /* Allocate more space: simple strategy – double or set exact size. */
    size_t new_cap = new_len * 2;
    if (new_cap < new_len)
        new_cap = new_len;            /* guard against overflow */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL)                /* allocation failed – abort */
        abort();

    cache = tmp;
    cache_cap = new_cap;
}

static void init_cache(void)
{
    if (cache_len > 0)  /* already initialized */
        return;

    /* Need at least 4 slots for the recurrence base. */
    size_t init_cap = 4;
    cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
    if (cache == NULL)
        abort();

    cache_cap = init_cap;
    cache_len = 4;

    /* Set the four known initial terms. */
    cache[0] = (seq_t)2;
    cache[1] = (seq_t)13;
    cache[2] = (seq_t)72;
    cache[3] = (seq_t)385;
}

/* Main sequence accessor. */
seq_t a(idx_t n)
{
    /* n is unsigned 64-bit; cast to size_t for array indices. */
    size_t idx = (size_t)n;

    init_cache();                       /* lazy initialization */

    if (idx < cache_len)
        return cache[idx];

    /* Extend the cache up to idx. */
    ensure_capacity(idx + 1);

    for (size_t i = cache_len; i <= idx; ++i)
    {
        /* Compute a(i) via the recurrence. */
        seq_t val = 6 * cache[i - 1]       /* 6*a(i-1) */
                   - 5 * cache[i - 2]       /* -5*a(i-2) */
                   + 8 * cache[i - 3]       /* +8*a(i-3) */
                   +  cache[i - 4];         /* +a(i-4) */

        cache[i] = val;
    }

    cache_len = idx + 1;
    return cache[idx];
}

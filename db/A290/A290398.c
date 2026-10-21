/*
 * A290398  Number of tiles in distance d from a given heptagon in the
 *          truncated order-3 tiling of the heptagonal plane (a.k.a. the
 *          "hyperbolic soccerball").
 * https://oeis.org/A290398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 37, u64 n <= 78, u128 n <= 160,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence:  a(0)=1, a(1)=7, a(2)=14, a(3)=28, a(4)=49
    For n ≥ 1:  a(n+4) = a(n+3) + a(n+2) + a(n+1) – a(n)   */
#include <stdlib.h>
#include <stddef.h>
#ifdef __cplusplus
#include <vector>
#endif

static void init_cache(void);

/* C version: dynamic array ------------------------------------------------- */
#ifndef __cplusplus
static seq_t *cache = NULL;      /* pointer to allocated array         */
static size_t capacity = 0;      /* current allocated capacity (elements) */
static size_t filled   = 0;      /* number of elements that have a value  */

/* initialise the first five terms */
static void init_cache(void)
{
    if (cache) return;                /* already initialised */
    cache = (seq_t*)malloc(5 * sizeof(seq_t));
    if (!cache) abort();
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)7;
    cache[2] = (seq_t)14;
    cache[3] = (seq_t)28;
    cache[4] = (seq_t)49;
    capacity = 5;
    filled   = 5;
}

/* ensure the array contains at least n+1 values, computing missing ones */
static void ensure_filled(idx_t n)
{
    if ((size_t)n < filled) return;            /* already have it */

    /* grow memory if necessary   */
    if ((size_t)n >= capacity) {
        size_t newcap = (capacity == 0) ? 5 : capacity;
        while (newcap <= (size_t)n) newcap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache    = tmp;
        capacity = newcap;
    }

    /* fill missing terms using the recurrence   */
    for (size_t i = filled; i <= (size_t)n; ++i) {
        /* safe addition/subtraction – uses seq_t operators           */
        cache[i] = cache[i-1] + cache[i-2] + cache[i-3] - cache[i-4];
    }
    filled = (size_t)n + 1;
}
#else /* __cplusplus --------------------------------------------------------- */
static std::vector<seq_t> cache_vec;

/* initialise the first five terms */
static void init_cache(void)
{
    if (!cache_vec.empty()) return;    /* already initialised */
    cache_vec.reserve(5);
    cache_vec.push_back((seq_t)1);
    cache_vec.push_back((seq_t)7);
    cache_vec.push_back((seq_t)14);
    cache_vec.push_back((seq_t)28);
    cache_vec.push_back((seq_t)49);
}

/* ensure the vector contains at least n+1 values, computing missing ones */
static void ensure_filled(idx_t n)
{
    if ((size_t)n < cache_vec.size()) return;   /* already have it */

    /* grow vector size if necessary                          */
    if (cache_vec.size() < (size_t)n + 1)
        cache_vec.reserve((size_t)n + 1);

    /* append missing terms using the recurrence               */
    for (size_t i = cache_vec.size(); i <= (size_t)n; ++i) {
        cache_vec.push_back(
            cache_vec[i-1] + cache_vec[i-2] + cache_vec[i-3] - cache_vec[i-4]
        );
    }
}
#endif /* __cplusplus */

/* public entry -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) { init_cache(); initialized = 1; }

    ensure_filled(n);
#ifndef __cplusplus
    return cache[(size_t)n];
#else
    return cache_vec[(size_t)n];
#endif
}

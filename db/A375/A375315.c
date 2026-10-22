/*
 * A375315  Expansion of (1 + x)/(1 - x^2*(1 + x)^3).
 * https://oeis.org/A375315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + 3*a(n-3) + 3*a(n-4) + a(n-5) for n >= 5, with
 *            initial terms: a(0) = 1, a(1) = 1, a(2) = 1, a(3) = 4, a(4) = 7
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 35, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

/* initial values to seed the recurrence */
static const seq_t init_vals[5] = { (seq_t)1, (seq_t)1, (seq_t)1,
                                    (seq_t)4, (seq_t)7 };

#ifdef __cplusplus
/* use std::vector for C++ builds */
static std::vector<seq_t> cache_vec;

/* ensure the vector is at least up_to+1 elements long */
static void ensure_cache(idx_t up_to)
{
    if (cache_vec.size() > (size_t)up_to) return;
    /* reserve extra space for amortized O(1) growth */
    size_t need = (size_t)up_to + 1;
    if (cache_vec.size() < 16) need = 16;
    if (need > 0 && cache_vec.capacity() < need) cache_vec.reserve(need * 2);
    cache_vec.resize(need);
    /* fill newly added entries with initial values if within first 5 indices */
    for (size_t i = 0; i < 5 && i < need; ++i) cache_vec[i] = init_vals[i];
}

static seq_t get_cached(idx_t idx)   { return cache_vec[(size_t)idx]; }
static void set_cached(idx_t idx, seq_t val) { cache_vec[(size_t)idx] = val; }

#else
/* use malloc/realloc for C builds */
static seq_t *cache = NULL;      /* array of computed terms */
static size_t cache_len = 0;     /* number of allocated slots */

/* ensure the array is at least up_to+1 elements long */
static void ensure_cache(idx_t up_to)
{
    size_t need = (size_t)up_to + 1;
    if (need <= cache_len) return;

    size_t new_len = cache_len ? cache_len * 2 : 16;
    if (new_len < need) new_len = need;
    seq_t *new_cache = realloc(cache, new_len * sizeof(seq_t));
    cache = new_cache;
    cache_len = new_len;

    /* initialise first five entries if they exist */
    if (cache_len > 0 && cache_len >= 5) {
        for (size_t i = 0; i < 5; ++i) cache[i] = init_vals[i];
    }
}

static seq_t get_cached(idx_t idx)   { return cache[(size_t)idx]; }
static void set_cached(idx_t idx, seq_t val) { cache[(size_t)idx] = val; }
#endif

seq_t a(idx_t n)
{
    if (n < 5) return init_vals[(size_t)n];   /* direct return for first five terms */

    /* Ensure we have computed all terms up to n */
    ensure_cache(n);

    /* Compute values from the first missing index up to n */
    idx_t start = 5;
    for (idx_t i = start; i <= n; ++i)
    {
        seq_t term = get_cached(i - 2)
                   + (seq_t)3 * get_cached(i - 3)
                   + (seq_t)3 * get_cached(i - 4)
                   + get_cached(i - 5);
        set_cached(i, term);
    }

    return get_cached(n);
}

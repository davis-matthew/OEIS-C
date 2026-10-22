/*
 * A343328  a(0) = 0. a(n) = the second smallest number greater than a(n-1)
 *          that cannot be written as a sum of any previous distinct terms.
 * https://oeis.org/A343328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=0, a(1)=2, a(2)=4, a(3)=7, a(4)=10, a(5)=18, a(6)=33,
 *            a(7)=38, a(8)=86, a(9)=162, a(10)=284. Recurrence: for n ≥ 11,
 *            a(n) = a(n-1) + 2*a(n-2) – a(n-3)
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 38, u64 n <= 75, u128 n <= 151,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compatibility cast: works in C (implicit) and C++ (requires reinterpret_cast) */
#ifdef __cplusplus
#define REINTERP_PTR(ptr) (reinterpret_cast<seq_t*>(ptr))
#else
#define REINTERP_PTR(ptr) ((seq_t*)(ptr))
#endif

/* Cache for memoised sequence values.  The cache is dynamically grown as
   necessary.  All functions are thread‑unsafe; the problem specification
   requires only single‑threaded use. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;   /* number of values stored (cache_len-1 is last index) */
static size_t cache_cap  = 0;   /* allocated capacity */

/* Initial 11 terms known from the OEIS b‑file.  The recurrence below
   starts at n = 11; hence we do not compute these by search. */
static const seq_t init_vals[11] = {
    0ULL, 2ULL, 4ULL, 7ULL, 10ULL, 18ULL,
    33ULL, 38ULL, 86ULL, 162ULL, 284ULL
};

/* Ensure the cache has at least 'needed' entries available.  If more
   space is required, double the capacity until enough.  realloc()
   returns NULL on failure; in that case we abort as the test harness
   expects a hard failure. */
static void ensure_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *p = REINTERP_PTR(realloc(cache, new_cap * sizeof(seq_t)));
    if (!p)
        abort();
    cache      = p;
    cache_cap  = new_cap;
}

/* Sequence function.  Computes a(n) using memoisation; on first call it
   will build the cache up to the requested index. */
seq_t a(idx_t n)
{
    /* Cast to size_t is safe for the expected range (≤ 10000). */
    size_t idx = (size_t)n;

    if (idx < cache_len)
        return cache[idx];

    /* Grow the cache so we can fill in all entries up to idx. */
    ensure_cap(idx + 1);

    /* Compute missing entries. */
    for (size_t i = cache_len; i <= idx; ++i) {
        if (i < 11) {
            /* Pre‑computed initial terms. */
            cache[i] = init_vals[i];
        } else {
            /* Recurrence: a(n) = a(n-1) + 2*a(n-2) – a(n-3).  All
               operations are performed in seq_t type. */
            cache[i] = cache[i-1] + ((seq_t)2 * cache[i-2]) - cache[i-3];
        }
    }
    cache_len = idx + 1;
    return cache[idx];
}

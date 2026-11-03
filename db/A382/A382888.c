/*
 * A382888  The squarefree kernel of the n-th cubefree number.
 * https://oeis.org/A382888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = squarefree kernel of the n‑th cubefree number. The code
 *            generates cubefree numbers in increasing order, caches their
 *            squarefree kernel, and returns the cached value for any n >= 1
 * Range:     u8 n <= 214, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 8566 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/*  Test whether x is cubefree.  If so, compute its squarefree kernel
    into *rad_out and return true; otherwise return false. */
static bool test_cubefree_and_rad(uint64_t x, seq_t *rad_out)
{
    uint64_t m = x;
    seq_t rad = (seq_t)1;

    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            int exp = 0;
            rad *= (seq_t)p;                /* include prime once */
            while (m % p == 0) {
                m /= p;
                exp++;
                if (exp > 2)                 /* found a cube factor */
                    return false;
            }
        }
    }
    if (m > 1)                            /* remaining prime factor */
        rad *= (seq_t)m;
    *rad_out = rad;
    return true;
}

static seq_t *cache = NULL;        /* cached a(1) .. a(cache_len-1) */
static size_t cache_cap = 0;       /* allocated capacity of cache */
static size_t cache_len = 0;       /* number of elements actually stored */
static uint64_t next_candidate = 1;/* next integer to test for cubefree */

/*  Ensure that cache can hold at least 'needed' elements. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;
    cache = seq_realloc(cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

/*  a(n) as defined above.  Computes values lazily and caches them. */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for n < 1 */
        return (seq_t)0;

    ensure_cache_capacity(n);

    while (cache_len < n) {
        seq_t rad;
        if (test_cubefree_and_rad(next_candidate, &rad)) {
            cache[cache_len++] = rad;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}

/*
 * A342855  Numbers that contain a digit 0 (A011540) and that are divisible by
 *          their nonzero digits (A002796).
 * https://oeis.org/A342855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms by searching positive integers that contain a digit 0
   and that are divisible by all their non‑zero digits. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#if defined(__cplusplus)
#include <new>
#endif

/* a(1) = 10, a(2) = 20, … .  The function is memoised and works
   for arbitrary call order. */

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Allocate a new array of seq_t objects.  In C++ we use 'new',
   in C we use malloc.  The array is initialised to zero. */
static seq_t *alloc_array(size_t cap)
{
#if defined(__cplusplus)
    seq_t *p = new seq_t[cap];
    for (size_t i = 0; i < cap; ++i) p[i] = 0;
    return p;
#else
    seq_t *p = (seq_t*)malloc(cap * sizeof(seq_t));
    if (p) {
        for (size_t i = 0; i < cap; ++i) p[i] = 0;
    }
    return p;
#endif
}

/* Grow the cache to at least 'new_cap' elements.  Elements beyond
   the old length are zeroed. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    seq_t *new_cache = alloc_array(new_cap);
    for (size_t i = 0; i < cache_len; ++i) new_cache[i] = cache[i];
    /* Free old cache */
#if defined(__cplusplus)
    delete[] cache;
#else
    free(cache);
#endif
    cache = new_cache;
    cache_cap = new_cap;
}

/* Generate terms until we have at least 'needed' entries. */
static void generate_terms(idx_t needed)
{
    if (cache == NULL) {
        cache_cap = 16;
        cache = alloc_array(cache_cap);
    }
    seq_t candidate = (cache_len == 0) ? (seq_t)10 : cache[cache_len - 1] + 1;

    while (cache_len < (size_t)needed) {
        bool hasZero = false;
        bool ok = true;
        seq_t temp = candidate;
        while (temp > 0) {
            unsigned int d = (unsigned int)(temp % 10);
            temp /= 10;
            if (d == 0) {
                hasZero = true;
                continue;
            }
            if (candidate % (seq_t)d != 0) {
                ok = false;
                break;
            }
        }
        if (hasZero && ok) {
            if (cache_len >= cache_cap) {
                grow_cache(cache_cap * 2 + 1);
            }
            cache[cache_len++] = candidate;
        }
        candidate++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1, undefined for n==0 */
    if ((size_t)n <= cache_len) return cache[n - 1];
    generate_terms(n);
    return cache[n - 1];
}

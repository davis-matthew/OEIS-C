/*
 * A365874  The "quadruple commas" sequence, a variant of A121805. See the
 *          Comments and Example sections for detailed explanations.
 * https://oeis.org/A365874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini, full, changed
 * Formula:   a(1) = 1, and for n > 1 the next term a(n) is the smallest
 *            integer a(n-1) < a(n) satisfying a(n) – a(n-1) = 4 * (10 *
 *            (a(n-1) mod 10) + first_digit(a(n))), where "first_digit" is the
 *            most significant decimal digit of a(n)
 * Range:     u8 n <= 2, u16 n <= 285, u32 n <= 44324, u64 n <= 44324,
 *            u128 n <= 44324, big any n
 * Validated: full, pass, 44324 terms through n = 44324 (2026-11-03)
 * Helpers:   mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#include <stdlib.h>
#include <stdint.h>
#include "../utils/num_digits.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Find the most significant decimal digit of an arbitrary-precision integer. */
static int first_digit(seq_t x)
{
    int len = num_digits(x);          /* number of decimal digits */
    seq_t base = pow10(len - 1);     /* 10^(len-1) */
    seq_t high = x / base;           /* truncating division */
    return (int)high;                /* high must be 0..9 */
}

/* Cache for computed terms. */
static seq_t *cache   = NULL;
static size_t cache_cap = 0;   /* allocated capacity */
static idx_t  cache_len  = 0;   /* number of terms stored */

/* Make sure terms up to idx n are computed and stored in cache. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len)
        return;

    /* Allocate space for n terms. */
    ensure_capacity((size_t)n, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));

    /* Compute missing terms. */
    for (idx_t k = cache_len + 1; k <= n; ++k)
    {
        if (k == 1)            /* a(1) = 1 */
        {
            cache[0] = 1;
            continue;
        }

        seq_t prev = cache[k - 2];           /* a(k-1) */
        seq_t last_digit = prev % 10;        /* L */

        seq_t best_candidate = 0;            /* 0 means not found yet */
        bool found = false;

        for (int first = 1; first <= 9; ++first)
        {
            seq_t m   = 10 * last_digit + first;           /* (10L + F) */
            seq_t diff = 4 * m;                            /* 4*(10L+F) */
            seq_t cand = prev + diff;                      /* a(k) candidate */

            if (first_digit(cand) == first)                /* satisfies rule */
            {
                if (!found || cand < best_candidate)
                {
                    best_candidate = cand;
                    found = true;
                }
            }
        }

        /* If no candidate was found, the sequence has ended. */
        if (!found)
        {
            cache_len = k - 1;
            return;
        }

        cache[k - 1] = best_candidate;
    }

    cache_len = n;
}

/* Public function as required. */
seq_t a(idx_t n)
{
    if (n < 1)                 /* undefined, but just return 0 */
        return 0;

    ensure_cache(n);
    return cache[n - 1];
}

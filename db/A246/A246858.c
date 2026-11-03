/*
 * A246858  Composite numbers k such that sigma(k + sigma(k)) = 2*sigma(k).
 * https://oeis.org/A246858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 35, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A246858: composite numbers k such that sigma(k + sigma(k)) = 2*sigma(k) */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

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

static idx_t       last_checked = 1;   /* last integer examined */
static size_t      cache_len   = 0;    /* number of terms stored */
static size_t      cache_cap   = 0;    /* capacity of the cache array */
static seq_t      *cache       = NULL; /* cached terms */

seq_t a(idx_t n)
{
    if (n == 0) /* safety – sequence is 1‑based, but callers may pass 0 */
        return 0;

    while (cache_len < n) {
        ++last_checked;

        /* Skip numbers smaller than 2 and primes, keeping only composites */
        if (last_checked < 2 || is_prime(last_checked))
            continue;

        seq_t sigma_k  = sigma_of(last_checked);

        /* Compute k + sigma(k) and its sigma */
        idx_t  target   = last_checked + (idx_t)sigma_k;
        seq_t  sigma_sum = sigma_of(target);
        seq_t  double_sigma = sigma_k + sigma_k;

        if (sigma_sum == double_sigma) {
            /* Grow cache if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                ensure_capacity(new_cap, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}

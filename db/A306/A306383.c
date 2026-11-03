/*
 * A306383  Number of ways to write n as x*(2x+1) + y*(2y+1) + z*(2z+1), where
 *          x,y,z are nonnegative integers with x <= y <= z.
 * https://oeis.org/A306383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of triples (x,y,z) of non‑negative integers
 *            satisfying x ≤ y ≤ z and n = x\*(2x+1) + y\*(2y+1) + z\*(2z+1)
 * Range:     u8 n <= 752, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 117 terms through n = 8486 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
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

static seq_t *hex_cache = NULL;      /* pre‑computed hexagonal numbers */
static size_t hex_cap   = 0;          /* current capacity of hex_cache */

seq_t a(idx_t n)
{
    /* convert n to seq_t for arithmetic */
    seq_t nseq = (seq_t)n;
    seq_t res  = 0;

    /* Build hexagonal numbers up to n (h(k)=k*(2k+1)) */
    idx_t k = 0;
    seq_t hk = 0;                      /* h(0) = 0 */
    while (hk <= nseq) {
        /* grow the cache if necessary */
        ensure_capacity((size_t)(k + 1), &hex_cap, OEISC_VPP(&hex_cache), sizeof(seq_t));
        hex_cache[k] = hk;
        ++k;
        /* compute next hexagonal number */
        hk = (seq_t)k * ((seq_t)2 * k + 1);
    }
    /* k now points to first index where h(k) > n, so max index = k-1 */
    if (k == 0) return 0;              /* n < 0 not possible, but guard */
    idx_t maxk = k - 1;

    /* Count triples (x,y,z) with 0 ≤ x ≤ y ≤ z ≤ maxk and h(x)+h(y)+h(z)=n */
    for (idx_t x = 0; x <= maxk; ++x) {
        seq_t hx = hex_cache[x];
        for (idx_t y = x; y <= maxk; ++y) {
            seq_t hy = hex_cache[y];
            if (hx + hy > nseq)           /* no further y possible */
                break;
            seq_t rem = nseq - hx - hy;   /* remaining value for z */

            if (rem < hy)                 /* z must be ≥ y */
                continue;
            if (rem > hex_cache[maxk])    /* exceeds maximum hexagonal number */
                continue;

            /* binary search for z satisfying h(z)=rem, z≥y */
            idx_t lo = y, hi = maxk;
            while (lo <= hi) {
                idx_t mid = lo + ((hi - lo) >> 1);
                seq_t hmid = hex_cache[mid];
                if (hmid == rem) {
                    ++res;               /* found a valid triple */
                    break;               /* only one z for given x,y */
                } else if (hmid < rem) {
                    lo = mid + 1;
                } else {
                    hi = mid - 1;
                }
            }
        }
    }

    return res;
}

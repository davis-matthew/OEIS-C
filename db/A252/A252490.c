/*
 * A252490  Numbers whose set of digits is simply connected, with 9 and 0
 *          considered as neighbors.
 * https://oeis.org/A252490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth positive integer whose set of digits is a single
 *            contiguous block on the cyclic digit set 0–9 (with 9 and 0
 *            considered neighbors)
 * Range:     u8 n <= 62, u16 n <= 3691, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 93 terms through n = 9511 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
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

/* Returns true iff the 10‑bit bitmask 'mask' represents a set of digits that is a contiguous
   block on the cycle 0‑9.  (digits are the positions of set bits). */
static bool contiguous_mask_valid(uint16_t mask)
{
    uint16_t complement = (~mask) & 0x3FFU;    /* digits not present */
    if (complement == 0)
        return true;                          /* all digits present – contiguous by default */

    int blocks = 0;
    for (int i = 0; i < 10; ++i) {
        if (complement & (1U << i)) {
            int prev = (i + 9) % 10;          /* previous position in the cycle */
            if (!(complement & (1U << prev))) {
                ++blocks;
                if (blocks > 1)
                    return false;             /* more than one zero block -> not contiguous */
            }
        }
    }
    return true;
}

/* Computes the bitmask of distinct decimal digits of x and tests it for contiguity. */
static bool number_is_valid(idx_t x)
{
    uint16_t mask = 0;
    while (x > 0) {
        uint8_t d = x % 10;                   /* digit 0–9 */
        mask |= 1U << d;
        x /= 10;
    }
    return contiguous_mask_valid(mask);
}

/* Main OEIS sequence routine. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                     /* not defined for n=0, return 0 as placeholder */

    static seq_t *cache      = NULL;         /* cached sequence values */
    static size_t cache_len = 0;             /* number of cached values */
    static size_t cache_cap = 0;             /* allocated capacity of cache array */
    static idx_t  next_num   = 1;            /* next natural number to test */

    /* Return cached value if available. */
    if ((size_t)n <= cache_len)
        return cache[n - 1];

    /* Generate new values until we have at least n terms. */
    while (cache_len < (size_t)n) {
        idx_t val = next_num++;
        if (number_is_valid(val)) {
            ensure_capacity(cache_len + 1, &cache_cap, OEISC_VPP(&cache),
                            sizeof(seq_t));
            cache[cache_len++] = (seq_t)val;
        }
    }
    return cache[n - 1];
}

/*
 * A252480  Numbers whose decimal representation has at least one '0' digit in
 *          a position other than the final digit.
 * https://oeis.org/A252480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9407 (2026-11-03)
 * Helpers:   get_digits, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose decimal representation has at least one '0'
   digit in a position other than the final digit.
   a(1) = 100, a(2) = 101, a(3) = 102, ... */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/get_digits.h"
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

static bool has_nonfinal_zero(seq_t x) {
    if (x < (seq_t)100) return false;
    size_t len;
    uint8_t digits[64];
    get_digits(x, digits, &len);
    /* Skip the least‑significant digit (index 0) */
    for (size_t i = 1; i < len; ++i) {
        if (digits[i] == 0) return true;
    }
    return false;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined offset behaviour not required */

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if ((size_t)n > cache_len) {
        seq_t candidate = cache_len ? cache[cache_len - 1] + 1 : (seq_t)99;
        while (cache_len < (size_t)n) {
            if (has_nonfinal_zero(candidate)) {
                ensure_capacity(cache_len + 1, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
                cache[cache_len++] = candidate;
            }
            candidate += 1;
        }
    }

    return cache[n - 1];
}

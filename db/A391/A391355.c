/*
 * A391355  Numbers k such that 4*k - (greatest prime < 4*k) = (least prime >
 *          4*k) - 4*k.
 * https://oeis.org/A391355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   for consecutive primes p and q with (p+q)%8==0, k = (p+q)/8
 * Range:     u8 n <= 9, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-03)
 * Helpers:   mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/next_prime.h"
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

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset is 1, no term for n==0 */

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t prev_prime = 0;

    /* initialize on first call */
    if (cache_len == 0) {
        prev_prime = 2;            /* first prime */
    }

    while (cache_len < n) {
        uint64_t next = next_prime(prev_prime);   /* next consecutive prime */
        if (((prev_prime + next) & 7ULL) == 0ULL) {
            seq_t val = (seq_t)(prev_prime + next) / 8ULL;
            ensure_capacity(cache_len + 1, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));
            cache[cache_len++] = val;
        }
        prev_prime = next;
    }

    return cache[n - 1];
}

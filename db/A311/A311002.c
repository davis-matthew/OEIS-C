/*
 * A311002  Coordination sequence Gal.3.3.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-5) - a(n-10) for n > 10; a(0)=1, a(1)=4, a(2)=8,
 *            a(3)=10, a(4)=16, a(5)=18, a(6)=24, a(7)=26, a(8)=28, a(9)=36,
 *            a(10)=36
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 49 (2026-11-03)
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

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t computed = 0;

    /* base values for n = 0..10 */
    static const seq_t base[11] = {1,4,8,10,16,18,24,26,28,36,36};

    size_t N = (size_t)n;

    /* Ensure capacity for index N */
    ensure_capacity(N + 1, &cache_cap, OEISC_VPP(&cache), sizeof(seq_t));

    /* Compute values up to N if not already cached */
    if (computed <= N) {
        for (size_t i = computed; i <= N; ++i) {
            if (i < 11) {
                cache[i] = base[i];
            } else {
                cache[i] = ( (seq_t)2 * cache[i-5] ) - cache[i-10];
            }
        }
        computed = N + 1;
    }

    return cache[N];
}

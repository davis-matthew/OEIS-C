/*
 * A246878  a(0) = 1, then a(n) = Sum_{k = floor(log_2(n)) .. n - 1} a(k).
 * https://oeis.org/A246878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A246878
 *   a(0) = 1;
 *   for n ≥ 1:
 *     if n is not a power of 2:   a(n) = 2 * a(n-1);
 *     if n is a power of 2 ≥ 2:    a(n) = 2 * a(n-1) - a(log2(n)-1).
 *   This is equivalent to the definition
 *   a(n) = Σ_{k = ⌊log2 n⌋}^{n-1} a(k).
 */

#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#ifdef __cplusplus
#include <algorithm>   /* std::copy */
#include <new>         /* ::operator new/delete */
#endif

/* Helper: compute ⌊log₂ v⌋ for v > 0.  (v == 0 never occurs.) */
static idx_t floor_log2_uint64(idx_t v)
{
    /* builtins are available in both C and C++ */
    return 63 - __builtin_clzll(v);
}

seq_t a(idx_t n)
{
    /* Static cache: cache[i] holds a(i) for 0 ≤ i ≤ last_computed. */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;            /* allocated capacity */
    static idx_t last  = 0;            /* highest index computed so far */

    /* Grow the cache if necessary. */
    if (n + 1 > cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp;
#ifdef __cplusplus
        tmp = new seq_t[new_cap];
        if (cache) {
            /* copy existing values */
            std::copy(cache, cache + cap, tmp);
            delete[] cache;
        }
#else
        tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
#endif
        if (!tmp) abort();            /* allocation failure */
        cache = tmp;
        /* initialise new slots to zero (only needed for seq_t that
           is trivial; harmless otherwise). */
        for (idx_t i = cap; i < new_cap; ++i)
            cache[i] = 0;
        cap = new_cap;
    }

    /* Initialise base cases a(0) and a(1). */
    if (cache[0] == 0)
        cache[0] = (seq_t)1;
    if (n >= 1 && cache[1] == 0)
        cache[1] = (seq_t)1;

    /* Compute values up to n. */
    if (last < 1)
        last = 1;   /* we have at least a(0) and a(1) */

    for (idx_t i = last + 1; i <= n; ++i) {
        if ((i & (i - 1)) == 0) {                 /* i == 2^k, k ≥ 1 */
            idx_t k = floor_log2_uint64(i);       /* k = log2(i) */
            cache[i] = cache[i-1] * (seq_t)2 - cache[k-1];
        } else {                                  /* i not a power of 2 */
            cache[i] = cache[i-1] * (seq_t)2;
        }
    }
    last = n;   /* cache now contains a(n) */

    return cache[n];
}

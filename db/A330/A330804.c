/*
 * A330804  Number of chains in partitions of [n] ordered by refinement.
 * https://oeis.org/A330804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Bell(n) + Sum_{i=1}^{n-1} Stirling2(n,i)*a(i)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 22,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 252 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    /* Static cache */
    static seq_t *a_cache = NULL;
    static idx_t a_cache_len = 0;     /* number of computed elements */
    static idx_t a_cache_cap = 0;     /* allocated capacity */

    static seq_t *prev = NULL;        /* previous Stirling row */
    static idx_t prev_len = 0;        /* length of previous row (n) */

    /* Initialisation */
    if (a_cache == NULL) {
        a_cache_cap = 1;
        a_cache = mem_new(seq_t, a_cache_cap);
        a_cache[0] = 1;              /* a(0) = 1 */
        a_cache_len = 1;
    }
    if (prev == NULL) {
        prev_len = 1;
        prev = mem_new(seq_t, prev_len);
        prev[0] = 1;                  /* S(0,0) = 1 */
    }

    /* Return if already computed */
    if (n < a_cache_len)
        return a_cache[n];

    /* Ensure capacity for cache array */
    if (n >= a_cache_cap) {
        idx_t new_cap = a_cache_cap ? a_cache_cap << 1 : 1;
        while (new_cap <= n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, a_cache, new_cap);
        a_cache = tmp;
        a_cache_cap = new_cap;
    }

    /* Compute successive values until n is reached */
    for (idx_t next_n = a_cache_len; next_n <= n; ++next_n) {
        /* Allocate current row (size next_n+1, filled with zeros) */
        seq_t *cur = mem_new(seq_t, (size_t)(next_n + 1));

        seq_t bell    = 0; /* Bell(next_n) */
        seq_t weighted = 0;/* Sum_{k=1}^{next_n-1} S(next_n,k)*a(k) */

        /* k from 1 to next_n-1 (if any) */
        for (idx_t k = 1; k < next_n; ++k) {
            seq_t val = mul_small(prev[k], (unsigned int)k) + prev[k - 1];
            cur[k] = val;
            bell += val;
            weighted += val * a_cache[k];
        }

        /* k = next_n (S(next_n, next_n) = S(next_n-1, next_n-1)) */
        seq_t val_last = prev[next_n - 1];
        cur[next_n] = val_last;
        bell += val_last;
        /* No weighted contribution for k = next_n */

        seq_t next_val = bell + weighted;

        /* Store computed a(next_n) */
        a_cache[next_n] = next_val;
        a_cache_len = next_n + 1;

        /* Prepare for next iteration */
        mem_free(prev);
        prev = cur;
        prev_len = next_n + 1;
    }

    return a_cache[n];
}

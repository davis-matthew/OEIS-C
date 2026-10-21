/*
 * A276553  Numbers k such that k^2 and (k + 1)^2 have the same number of
 *          divisors.
 * https://oeis.org/A276553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 6419, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   tau_square_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A276553
 *  Numbers k such that k^2 and (k+1)^2 have the same number of divisors.
 *  1‑based offset.  The function caches terms so that calls are
 *  independent of order.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/tau_square_uint64.h"

/* --------------------------------------------------------------------- */
/*  Dynamic array growth helper                                         */
static void grow_array(size_t *cap, size_t needed, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = *cap ? (*cap << 1) : 2;
    while (new_cap < needed) new_cap <<= 1;
    void *p = realloc(*ptr, new_cap * elem_size);
    if (!p) abort();
    *ptr = p;
    *cap = new_cap;
}

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/*  Cache for discovered k’s                                             */
static seq_t *cache     = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t   scan_pos = 1;   /* next k to examine */

/* --------------------------------------------------------------------- */
/*  Ensure that at least target terms are cached                        */
static void ensure_terms(idx_t target)
{
    while (cache_len < target) {
        /* compute τ(k²) and τ((k+1)²) */
        uint64_t k  = (uint64_t)scan_pos;
        uint64_t kp = k + 1;
        seq_t tau_k  = tau_square_uint64(k);
        seq_t tau_kp = tau_square_uint64(kp);

        if (tau_k == tau_kp) {
            grow_array(&cache_cap, cache_len + 1, (void **)&cache, sizeof(seq_t));
            cache[cache_len++] = (seq_t)scan_pos;
        }
        ++scan_pos;
    }
}

/* --------------------------------------------------------------------- */
/*  Sequence function                                                   */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset 1 – callers should use n≥1 */
    ensure_terms(n);
    return cache[n - 1];
}

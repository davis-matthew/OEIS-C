/*
 * A249804  a(n+1) is the next smallest nontrivial cube beginning with a(n),
 *          initial term is 4.
 * https://oeis.org/A249804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Validated: standard, pass, 7 terms through n = 7 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Description: a(1) = 4.
   For n≥1, a(n+1) is the smallest *non‑trivial* cube > a(n) whose
   decimal representation begins with that of a(n).  "Non‑trivial" means
   that the cube’s integer root is not divisible by 10 (i.e., the cube
   does not end in zeros). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include <string.h>
#include <stddef.h>
#include <math.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Cached sequence values */
static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_cap   = 0;

/* Ensure the cache can store at least `need` terms */
static void ensure_cache(idx_t need)
{
    if (need <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_cache;
    if (!cache)
        new_cache = seq_alloc_new_array(new_cap);
    else
        new_cache = seq_realloc(cache, cache_cap, new_cap);
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the next cube that starts with `prev` and whose root is not
   a multiple of 10 (non‑trivial). */
static seq_t next_cube_with_prefix(seq_t prev)
{
    for (unsigned k = 0; ; ++k) {
        seq_t pow10k = pow10((idx_t)k);
        seq_t lower  = prev * pow10k;
        seq_t upper  = (prev + (seq_t)1) * pow10k - (seq_t)1;

        /* Determine the cube‑root bounds for this prefix interval */
        seq_t r_low  = floor_cube_root(lower - (seq_t)1) + (seq_t)1;
        seq_t r_high = floor_cube_root(upper);

        while (r_low <= r_high) {
            if (r_low % (seq_t)10 == 0) {   /* skip trivial cubes */
                ++r_low;
                continue;
            }
            seq_t cube = r_low * r_low * r_low;
            if (cube > prev) return cube;
            ++r_low;
        }
        /* No cube found in this range – increase the number of trailing
           zeros and try again. */
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined for n=0 */

    ensure_cache(n);

    while (cache_size < n) {
        if (cache_size == 0) {
            cache[0] = (seq_t)4;            /* a(1) = 4 */
        } else {
            seq_t prev = cache[cache_size - 1];
            cache[cache_size] = next_cube_with_prefix(prev);
        }
        ++cache_size;
    }
    return cache[n - 1];
}

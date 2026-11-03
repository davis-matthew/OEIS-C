/*
 * A249837  a(n+1) is the next smallest nontrivial cube beginning with a(n),
 *          initial term is 9.
 * https://oeis.org/A249837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Validated: standard, pass, 6 terms through n = 6 (2026-11-01)
 * Helpers:   ipow_i32, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n+1) is the next smallest nontrivial cube beginning with a(n)
   (nontrivial means the cube root is not divisible by 10) */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/ipow_i32.h"
#include "../utils/ipow_idx.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
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

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

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

/* Return the smallest nontrivial cube whose decimal representation starts
   with the digits of `prev`.  The cube must be strictly larger than
   `prev`. */
static inline seq_t
next_cube_with_prefix(seq_t prev)
{
    /* We search over leading‑digit blocks 10^k.  For each block we
       find the smallest cube ≥ prev·10^k that lies in
       [prev·10^k , (prev+1)·10^k).  We skip cubes whose root is
       divisible by 10. */
    seq_t pow10_t = (seq_t)1;
    for (;;) {
        seq_t L = seq_mul(prev, pow10_t);                    /* prev·10^k */
        seq_t U = seq_mul(prev + (seq_t)1, pow10_t);         /* (prev+1)·10^k */

        /* Smallest integer `x` with x^3 ≥ L. */
        seq_t x = floor_cube_root(L);
        seq_t cube = ipow_idx(x, (idx_t)3);
        while (cube < L) {
            ++x;
            cube = ipow_idx(x, (idx_t)3);
        }

        /* Skip any cube whose root is divisible by 10. */
        while (cube < U && (x % (seq_t)10) == (seq_t)0) {
            ++x;
            cube = ipow_idx(x, (idx_t)3);
        }

        /* If a valid cube is found, return it. */
        if (cube < U && cube > prev)
            return cube;

        /* Try the next digit block. */
        pow10_t = seq_mul(pow10_t, (seq_t)10);
    }
}

/* The required sequence function. */
seq_t
a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;   /* undefined for n<1 */

    /* Static cache of computed terms. */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static idx_t computed = 0;            /* highest index already computed */

    /* Initial allocation. */
    if (cache == NULL) {
        capacity = 8;
        cache = seq_alloc(capacity);
        computed = 0;
    }

    /* Grow cache if necessary. */
    if ((idx_t)capacity < n) {
        size_t new_cap = capacity * 2;
        if ((idx_t)new_cap < n) new_cap = (size_t)n + 8;
        cache = seq_realloc(cache, capacity, new_cap);
        capacity = new_cap;
    }

    /* Compute any missing terms. */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i == (idx_t)1) {
            cache[i - 1] = (seq_t)9;
        } else {
            cache[i - 1] = next_cube_with_prefix(cache[i - 2]);
        }
    }
    computed = n;

    return cache[n - 1];
}

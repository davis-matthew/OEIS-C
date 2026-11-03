/*
 * A396011  Number of 2's in the partitions of n into exactly 5 parts.
 * https://oeis.org/A396011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)-2*a(n-5)+a(n-8)+a(n-9)-a(n-10) for n>20;
 *            Base values: a(0..20) as listed, then recurrence
 * Range:     u8 n <= 30, u16 n <= 208, u32 n <= 8517, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9406 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

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

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t capacity = 0;
    static idx_t computed = 0;  /* highest index computed (0‑based); 0 means none */

    /* Base values for n = 0..20 */
    static const seq_t base_vals[21] = {
        0, 0, 0, 0, 0, 0, 1, 2, 4, 7,
        12, 13, 18, 22, 28, 33, 41, 47, 57,
        65, 76
    };

    /* Ensure cache large enough for the requested n */
    if (n >= capacity) {
        idx_t new_cap = n + 1;
        if (cache == NULL) {
            cache = seq_alloc(new_cap);
            capacity = new_cap;
        } else {
            cache = seq_realloc(cache, capacity, new_cap);
            capacity = new_cap;
        }
    }

    /* Compute missing values up to n (non‑decreasing order) */
    for (idx_t i = computed + 1; i <= n; ++i) {
        if (i <= 20) {
            cache[i] = base_vals[(int)i];
        } else {
            /* recurrence: a(i) = a(i-1)+a(i-2)-2*a(i-5)+a(i-8)+a(i-9)-a(i-10) */
            seq_t sum = cache[i-1] + cache[i-2] + cache[i-8] + cache[i-9];
            seq_t sub = cache[i-5] + cache[i-5] + cache[i-10];
            cache[i] = sum - sub;
        }
    }
    computed = n;
    return cache[n];
}

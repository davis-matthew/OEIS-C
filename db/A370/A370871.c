/*
 * A370871  Numbers m such that c(0) >= c(1) > c(2), where c(k) = number of
 *          k's in the ternary representation of m.
 * https://oeis.org/A370871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer m such that in base‑3 representation, the
 *            counts satisfy c(0) >= c(1) > c(2) (where c(k) is the number of
 *            k's in the ternary digits of m)
 * Range:     u8 n <= 39, u16 n <= 9257, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 8523 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
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

static seq_t *cache = NULL;
static size_t cache_cap = 0;     /* allocated size of cache */
static size_t cache_len = 0;     /* number of elements stored */
static seq_t next_candidate = 1; /* next integer to test */

static void grow_cache(void)
{
    size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    if (tmp == NULL) {
        /* allocation failure – abort */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* n is 1‑based; ensure cache holds at least n elements */
    while (cache_len < n) {
        /* test next_candidate */
        seq_t m = next_candidate;
        next_candidate++;        /* increment for next round */

        /* count digits in base 3 */
        unsigned int c[3] = {0, 0, 0};
        seq_t tmp = m;
        if (tmp == 0) {
            c[0] = 1;
        } else {
            while (tmp != 0) {
                unsigned int d = (unsigned int)(tmp % 3);
                c[d]++;
                tmp /= 3;
            }
        }

        /* check condition c0 >= c1 && c1 > c2 */
        if (c[0] >= c[1] && c[1] > c[2]) {
            if (cache_len == cache_cap)
                grow_cache();
            cache[cache_len++] = m;
        }
    }
    return cache[n - 1];
}

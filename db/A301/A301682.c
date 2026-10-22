/*
 * A301682  Coordination sequence for node of type V1 in "krg" 2-D tiling (or
 *          net).
 * https://oeis.org/A301682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-3) - a(n-6) for n ≥ 7 a(0)=1, a(1)=6, a(2)=6,
 *            a(3)=18, a(4)=18, a(5)=18, a(6)=36
 * Range:     u8 n <= 44, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Grow a dynamic array of seq_t values, compatible with both C and C++.
   In C++ the array elements are default-constructed; in C they are
   left uninitialized but will be filled before use. */
static void grow_cache(seq_t **ptr, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t new_cap = (*cap) ? (*cap) * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *new_ptr;
#ifdef __cplusplus
    /* C++ : allocate a new array, copy old elements, delete old one */
    new_ptr = new seq_t[new_cap];
    if (*ptr) {
        for (size_t i = 0; i < *cap; ++i) new_ptr[i] = (*ptr)[i];
        delete[] *ptr;
    }
#else
    /* C : use realloc (memory copy handled automatically) */
    new_ptr = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
    if (!new_ptr && new_cap) /* realloc failed but requested non-zero size */
        exit(EXIT_FAILURE);
#endif
    *ptr  = new_ptr;
    *cap  = new_cap;
}

/* The coordinate function that implements the OEIS sequence. */
seq_t a(idx_t n)
{
    /* static dynamic cache */
    static seq_t *cache  = NULL;
    static size_t cache_cap = 0;
    static idx_t  computed_to = 0;   /* largest n for which cache[n] is set */

    /* ensure the cache can hold at least the first 7 entries */
    grow_cache(&cache, &cache_cap, 7);

    /* initialize base values once */
    if (computed_to < 6) {
        static const seq_t base[7] = {
            (seq_t)1,
            (seq_t)6,
            (seq_t)6,
            (seq_t)18,
            (seq_t)18,
            (seq_t)18,
            (seq_t)36
        };
        for (size_t i = 0; i < 7; ++i) cache[i] = base[i];
        computed_to = 6;
    }

    /* extend cache to position n, computing new terms on the fly */
    if (n > computed_to) {
        grow_cache(&cache, &cache_cap, (size_t)n + 1);
        for (size_t i = (size_t)(computed_to + 1); i <= (size_t)n; ++i) {
            /* recurrence a(i) = 2*a(i-3) - a(i-6)  for i >= 7 */
            seq_t val = 2 * cache[i - 3];
            val -= cache[i - 6];
            cache[i] = val;
        }
        computed_to = n;
    }

    return cache[n];
}

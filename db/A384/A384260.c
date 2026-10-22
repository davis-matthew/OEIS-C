/*
 * A384260  Consecutive internal states of the linear congruential
 *          pseudo-random number generator 3373*s mod (2^24-3) when started at
 *          s=1.
 * https://oeis.org/A384260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (3373 * a(n-1)) mod (2^24-3), a(1)=1
 */
/*  a(1) = 1
    a(2) = 3373
    a(3) = 11377129
    a(4) = 5569986
    ...
 *   (period 2^22-1)
 */
#include <stdlib.h>

static const seq_t MOD  = 16777213;   /* 2^24-3 */
static const seq_t MULT = 3373;

seq_t a(idx_t n) {
    /* static cache to service out‑of‑order calls */
    static seq_t *cache = NULL;      /* 1‑based index, cache[0] unused */
    static size_t capacity = 0;     /* allocated size */
    static idx_t max_index = 0;     /* highest index computed */

    if (n == 0) return (seq_t)0;   /* below offset; spec says behavior doesn't matter */

    if (cache == NULL) {
        /* initialise cache with space for at least index 1 */
        capacity = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * capacity);
        if (!cache) __builtin_trap();          /* abort on allocation failure */
        cache[1] = (seq_t)1;                   /* a(1) = 1 */
        max_index = 1;
    }

    /* extend cache up to n if necessary */
    if (n > max_index) {
        /* ensure capacity */
        if (n + 1 > capacity) {
            size_t new_cap = capacity;
            while (n + 1 > new_cap) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) __builtin_trap();
            cache = tmp;
            capacity = new_cap;
        }
        /* compute next terms iteratively */
        for (idx_t i = max_index + 1; i <= n; ++i) {
            cache[i] = (cache[max_index] * MULT) % MOD;
            ++max_index;
        }
    }

    return cache[n];
}

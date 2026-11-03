/*
 * A243869  Expansion of x^4/[(1+x)*Product_{k=1..3} (1-k*x)].
 * https://oeis.org/A243869
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^n - 4*2^n + (-1)^n + 6)/24. Implemented via the linear
 *            recurrence a(n) = 5*a(n-1) - 5*a(n-2) - 5*a(n-3) + 6*a(n-4) with
 *            initial values a(4)=1, a(5)=5, a(6)=20, a(7)=70
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 23, u64 n <= 43, u128 n <= 83,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 981 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

/* Helper to grow a sequence array of type seq_t. */
static void grow_seq_array(seq_t **arr, size_t *size, size_t new_len)
{
    if (*size >= new_len) return;
    size_t new_cap = (*size > 0) ? (*size << 1) : 16;
    while (new_cap < new_len) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *size = new_cap;
}

seq_t a(idx_t n)
{
    /* a(n) is defined only for n >= 4; for smaller indices return 0. */
    if (n < 4) {
        return 0;
    }

    /* static cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;      /* allocated size (number of elements) */
    static idx_t  computed_upto = 7;   /* largest index that has been computed */

    idx_t needed = n + 1;               /* we need indices 0..n inclusive */

    /* initial allocation */
    if (!cache) {
        cache_size = needed > 8 ? needed : 8;  /* at least 8 slots for a(4)..a(7) */
        cache = alloc_seq_array(cache_size);
        /* initialise the known start values */
        cache[4] = 1;
        cache[5] = 5;
        cache[6] = 20;
        cache[7] = 70;
        /* no need to set indices <4 as they are never used */
    }
    /* grow cache if necessary */
    else if (needed > (idx_t)cache_size) {
        grow_seq_array(&cache, &cache_size, (size_t)needed);
    }

    /* fill in any missing values up to n using the recurrence */
    idx_t start = computed_upto + 1;
    if (start < 8) start = 8;          /* our recurrence only applies for i >= 8 */

    for (idx_t i = start; i <= n; ++i) {
        seq_t v = (seq_t)5 * cache[i-1];
        v -= (seq_t)5 * cache[i-2];
        v -= (seq_t)5 * cache[i-3];
        v += (seq_t)6 * cache[i-4];
        cache[i] = v;
    }
    computed_upto = n > computed_upto ? n : computed_upto;

    return cache[n];
}

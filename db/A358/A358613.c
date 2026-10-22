/*
 * A358613  a(n) = Sum_{k=0..floor(n/3)} (-1)^k * (n-k)!/(k! * (n-3*k)!).
 * https://oeis.org/A358613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 14, i32 n <= 22, i64 n <= 40, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Sum_{k=0..floor(n/3)} (-1)^k * (n-k)!/(k! (n-3*k)!)
 * Recurrence: a(n) = (4*a(n-1) - a(n-2) - 2*(2*n-3)*a(n-3)) / 3  for n > 2
 * Base values: a(0)=1, a(1)=1, a(2)=1
 */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache to store previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;     /* number of allocated slots */
    static idx_t computed_up_to = 0; /* values computed in cache[0 .. computed_up_to-1] */

    /* ensure the cache can hold up to index n */
    if (n >= cache_size) {
        size_t new_cap = cache_size ? cache_size : 8;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* out of memory */
        cache = tmp;
        cache_size = (idx_t)new_cap;
    }

    /* initialise base cases if not already done */
    if (computed_up_to == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        computed_up_to = 3;
    }

    /* compute values up to n if needed */
    while (computed_up_to <= n) {
        idx_t i = computed_up_to;
        /* recurrence: a(i) = (4*a(i-1) - a(i-2) - 2*(2*i-3)*a(i-3)) / 3 */
        seq_t coeff = (seq_t)2 * ((seq_t)2 * (seq_t)i - (seq_t)3);
        seq_t val = ( (seq_t)4 * cache[i-1] - cache[i-2] - coeff * cache[i-3] ) / (seq_t)3;
        cache[i] = val;
        ++computed_up_to;
    }

    return cache[n];
}

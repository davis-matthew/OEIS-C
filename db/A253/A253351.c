/*
 * A253351  Number of (2+1) X (n+1) 0..1 arrays with every 2 X 2 subblock
 *          diagonal maximum minus antidiagonal maximum nondecreasing
 *          horizontally, vertically and ne-to-sw antidiagonally.
 * https://oeis.org/A253351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A253351
   Empirical recurrence: a(n) = 5*a(n-1) - 12*a(n-3) for n > 6
   Initial values:
   a(1) = 47, a(2) = 173, a(3) = 724, a(4) = 3160,
   a(5) = 13810, a(6) = 60368
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;      /* dynamic cache of computed terms */
static size_t cache_size = 0;    /* number of allocated entries (index 0 unused) */

/* ensure cache contains terms up to index n */
static void ensure_cache(idx_t n) {
    if ((size_t)n < cache_size)
        return;

    size_t new_size = (size_t)n + 1;               /* indices 0..n */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (tmp == NULL)
        exit(EXIT_FAILURE);                       /* memory allocation failure */

    cache = tmp;
    for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
        if (i == 0) continue;                     /* index 0 unused */
        else if (i == 1) cache[1] = (seq_t)47;
        else if (i == 2) cache[2] = (seq_t)173;
        else if (i == 3) cache[3] = (seq_t)724;
        else if (i == 4) cache[4] = (seq_t)3160;
        else if (i == 5) cache[5] = (seq_t)13810;
        else if (i == 6) cache[6] = (seq_t)60368;
        else /* i > 6 */ 
            cache[i] = (seq_t)5 * cache[i - 1] - (seq_t)12 * cache[i - 3];
    }
    cache_size = new_size;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* sequence is only defined for n >= 1 */
    ensure_cache(n);
    return cache[n];
}

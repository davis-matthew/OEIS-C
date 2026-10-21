/*
 * A274161  Numbers n such that in the edge-delete game on the path P_{n} the
 *          first player does not have a winning strategy.
 * https://oeis.org/A274161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A274161: a(n) = the n-th positive integer that is either
   2, 3, 17, 37, or congruent to 7, 11, 23, 27, or 31 modulo 34.
   The function generates terms in increasing order and caches them
   for repeated calls. */
#include <stdlib.h>
#include <stdbool.h>

/* static cache for computed terms */
static seq_t *cache = NULL;        /* array of computed terms        */
static idx_t cache_size = 0;       /* number of terms stored         */
static idx_t cache_cap  = 0;       /* capacity of the array          */
static idx_t last_checked = 1;     /* largest integer examined so far */

/* test whether x is in the set described above */
static inline bool is_member(idx_t x)
{
    if (x == 2 || x == 3 || x == 17 || x == 37)
        return true;
    seq_t mod = x % 34;
    return (mod == 7 || mod == 11 || mod == 23 || mod == 27 || mod == 31);
}

/* enlarge cache capacity if necessary */
static void grow_cache(idx_t needed)
{
    if (cache_cap >= needed)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (new_cap < needed)
        new_cap *= 2;
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    cache_cap = new_cap;
}

/* external function for OEIS sequence A274161 */
seq_t a(idx_t n)
{
    if (n == 0)          /* not used; offset is 1 */
        return 0;

    /* generate terms until we have at least n of them */
    if (cache_size < n) {
        grow_cache(n);
        while (cache_size < n) {
            last_checked++;
            if (is_member(last_checked))
                cache[cache_size++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}

/*
 * A380821  Length of the short leg in the unique primitive Pythagorean triple
 *          whose inradius is A000032(n) and such that its long leg and its
 *          hypotenuse are consecutive natural numbers.
 * https://oeis.org/A380821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=5, a(1)=3, a(2)=7; a(n)=2*a(n-1) – a(n-3) for n ≥ 3 (OEIS
 *            A380821)
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 182,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;
static size_t  cache_len = 0;   /* number of elements stored (cache_len-1 is largest index) */
static size_t  cache_cap = 0;   /* current allocation size */

/* Ensure cache holds values up to index n (inclusive). */
static void grow_cache_to(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (cache_len >= need)
        return;                     /* already big enough */

    /* enlarge allocation if necessary */
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < need)
            new_cap = need;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise first elements if cache empty */
    if (cache_len == 0) {
        if (need > 0) cache[0] = 5;
        if (need > 1) cache[1] = 3;
        if (need > 2) cache[2] = 7;
        cache_len = (need < 3) ? need : 3;
    }

    /* fill remaining entries */
    for (size_t i = cache_len; i < need; ++i) {
        seq_t val;
        if (i == 0)        val = 5;
        else if (i == 1)   val = 3;
        else if (i == 2)   val = 7;
        else               val = 2 * cache[i-1] - cache[i-3];
        cache[i] = val;
    }
    cache_len = need;
}

seq_t a(idx_t n)
{
    grow_cache_to(n);
    return cache[(size_t)n];
}

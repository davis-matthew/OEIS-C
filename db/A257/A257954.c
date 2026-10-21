/*
 * A257954  Heptagonal numbers (A000566) that are the sum of seven consecutive
 *          heptagonal numbers.
 * https://oeis.org/A257954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            a(n-1)+268510893235202*a(n-2)-268510893235202*a(n-3)-a(n-4)+a(n-5)
 *            Initial values: a(1)=380300389, a(2)=13416840252277,
 *            a(3)=102114787991863121805337,
 *            a(4)=3602567760523753992917728705,
 *            a(5)=27418932936218445934971843788960252365
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 2, u128 n <= 5,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/seq_from_str.h"

/* Ensure the dynamic array has at least the requested capacity */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        if (new_cap < needed) new_cap = needed;
        void *tmp = realloc(*ptr, new_cap * elem_size);
        *ptr   = tmp;
        *cap   = new_cap;
    }
}

/* seq_t a(idx_t n) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* outside the offset */

    /* Static initialization of the first five terms */
    static int init_done = 0;
    static seq_t init[5];
    if (!init_done) {
        init[0] = seq_from_str("380300389");
        init[1] = seq_from_str("13416840252277");
        init[2] = seq_from_str("102114787991863121805337");
        init[3] = seq_from_str("3602567760523753992917728705");
        init[4] = seq_from_str("27418932936218445934971843788960252365");
        init_done = 1;
    }

    /* Cache for already computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Ensure cache has at least the first five terms */
    if (cache_len == 0) {
        ensure_capacity(5, &cache_cap, (void **)&cache, sizeof(seq_t));
        for (size_t i = 0; i < 5; ++i) cache[i] = init[i];
        cache_len = 5;
    } else if (cache_len < 5) {
        for (size_t i = cache_len; i < 5; ++i) cache[i] = init[i];
        cache_len = 5;
    }

    /* Coefficient as seq_t */
    static int coeff_done = 0;
    static seq_t coeff;
    if (!coeff_done) {
        coeff = seq_from_str("268510893235202");
        coeff_done = 1;
    }

    /* If the requested index is already in the cache, return it */
    if ((size_t)n <= cache_len) {
        return cache[n-1];
    }

    /* Grow the cache to accommodate n */
    ensure_capacity((size_t)n, &cache_cap, (void **)&cache, sizeof(seq_t));

    /* Compute terms from cache_len up to n */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        /* i corresponds to a(i+1) in OEIS numbering */
        seq_t a1 = cache[i-1];
        seq_t a2 = cache[i-2];
        seq_t a3 = cache[i-3];
        seq_t a4 = cache[i-4];
        seq_t a5 = cache[i-5];

        seq_t val = a1;
        val += coeff * a2;
        val -= coeff * a3;
        val -= a4;
        val += a5;

        cache[i] = val;
    }
    cache_len = n;

    return cache[n-1];
}

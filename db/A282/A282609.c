/*
 * A282609  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 489", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A282609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-2)+a(n-4)-4*a(n-6) for n>13. Initial values:
 *            a(0)=1, a(1)=0, a(2)=7, a(3)=2, a(4)=27, a(5)=30, a(6)=103,
 *            a(7)=90, a(8)=315, a(9)=94, a(10)=1255, a(11)=346, a(12)=4923,
 *            a(13)=1630
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static const seq_t init[14] = {1,0,7,2,27,30,103,90,315,94,1255,346,4923,1630};

    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t) * 14);
        if (cache == NULL) return (seq_t)0;
        cap = 14;
        for (size_t i = 0; i < 14; ++i) cache[i] = init[i];
    }

    if (n < (idx_t)cap) return cache[n];

    size_t new_cap = (size_t)n + 1;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (new_cache == NULL) return (seq_t)0;
    cache = new_cache;

    for (size_t i = cap; i <= (size_t)n; ++i) {
        if (i < 14) {
            cache[i] = init[i];
        } else {
            cache[i] = 4*cache[i-2] + cache[i-4] - 4*cache[i-6];
        }
    }
    cap = new_cap;
    return cache[n];
}

/*
 * A283217  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 597", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A283217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + 16*a(n-4) - 16*a(n-6) for n>5; * initial values:
 *            a(0)=1, a(1)=1, a(2)=1, a(3)=13, a(4)=17, a(5)=29, a(6)=17
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    static bool init = false;
    static seq_t *cache = NULL;
    static idx_t cap = 0;
    static idx_t last = 0;

    if (!init) {
        /* allocate space for first 7 values */
        cap = 7;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)13;
        cache[4] = (seq_t)17;
        cache[5] = (seq_t)29;
        cache[6] = (seq_t)17;
        last = 6;
        init = true;
    }

    if (n >= cap) {
        idx_t newcap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        for (idx_t i = cap; i < newcap; ++i)
            cache[i] = (seq_t)0;
        cap = newcap;
    }

    if (n <= last)
        return cache[n];

    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = cache[i-2] + (seq_t)16 * cache[i-4] - (seq_t)16 * cache[i-6];
    }
    last = n;
    return cache[n];
}

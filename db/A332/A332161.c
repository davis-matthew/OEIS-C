/*
 * A332161  a(n) = 6*(10^(2*n+1)-1)/9 - 5*10^n.
 * https://oeis.org/A332161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 6*(10^(2*n+1)-1)/9 - 5*10^n
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 111*a(n-1) - 1110*a(n-2) + 1000*a(n-3); a(0)=1, a(1)=616, a(2)=66166; */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t capacity = 0;   /* capacity of allocated array */
    static idx_t used = 0;       /* number of computed terms */

    /* Ensure we have space for [0..n] */
    if (capacity < n + 1) {
        idx_t new_cap = capacity ? capacity * 2 : 3;
        while (new_cap < n + 1)
            new_cap *= 2;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache)
            abort(); /* allocation failure */
        if (cache) {
            memcpy(new_cache, cache, used * sizeof(seq_t));
            free(cache);
        }
        cache = new_cache;
        capacity = new_cap;
    }

    /* Initialise first three terms once */
    if (used < 3) {
        if (used <= 0 && capacity >= 1) cache[0] = 1;
        if (used <= 1 && capacity >= 2) cache[1] = 616;
        if (used <= 2 && capacity >= 3) cache[2] = 66166;
        used = 3;
    }

    /* Compute missing terms up to n */
    if (used <= n) {
        for (idx_t i = used; i <= n; ++i) {
            if (i == 0) cache[i] = 1;
            else if (i == 1) cache[i] = 616;
            else if (i == 2) cache[i] = 66166;
            else {
                seq_t t = cache[i-1] * (seq_t)111;
                t += cache[i-3] * (seq_t)1000;
                t -= cache[i-2] * (seq_t)1110;
                cache[i] = t;
            }
        }
        used = n + 1;
    }

    return cache[n];
}

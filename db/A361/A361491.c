/*
 * A361491  Expansion of x*(1+38*x+x^2)/((1-x)*(x^2-34*x+1)).
 * https://oeis.org/A361491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 35*a(n-1) - 35*a(n-2) + a(n-3) with a(1)=1, a(2)=73,
 *            a(3)=2521
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t length = 0;   /* highest index computed */
    static idx_t capacity = 0; /* allocated capacity (>= length+1) */

    if (n < 1) return 0; /* undefined for n < 1 */

    /* Ensure capacity for indices up to n */
    if (capacity < n + 1) {
        idx_t new_cap = capacity ? capacity << 1 : 8;
        while (new_cap < n + 1) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        capacity = new_cap;
    }

    /* Initialize first three terms if not already */
    if (length == 0) { cache[1] = (seq_t)1; length = 1; }
    if (length < 2) { cache[2] = (seq_t)73; length = 2; }
    if (length < 3) { cache[3] = (seq_t)2521; length = 3; }

    /* Compute missing terms up to n */
    for (idx_t i = length + 1; i <= n; ++i) {
        cache[i] = cache[i-1] * 35 - cache[i-2] * 35 + cache[i-3];
    }
    length = n;

    return cache[n];
}

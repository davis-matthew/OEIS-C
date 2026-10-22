/*
 * A364170  Related to expression as an alternating sum of k-th powers.
 * https://oeis.org/A364170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 8, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1
   a(2) = 3
   a(3) = 6
   a(n) = ((a(n-1)+2)/2)^2 - a(n-1)  for n>=4
*/
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t size = 0;              /* highest index currently stored */

    if (n == 0) return 0;                /* not used, but safe */

    if (cache == NULL) {                 /* first call – allocate and seed */
        capacity = 4;                    /* enough for indices 1..3 */
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) return 0;            /* allocation failed */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)6;
        size = 3;
    }

    /* Ensure the array can hold index n */
    if ((size_t)n >= capacity) {
        size_t new_capacity = ((size_t)n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        if (!tmp) return cache[size];   /* allocation failed – return last value */
        cache = tmp;
        capacity = new_capacity;
    }

    /* Compute terms up to n if not already present */
    if ((size_t)n > size) {
        for (idx_t i = (idx_t)(size + 1); i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t t = (prev + (seq_t)2) >> 1;  /* (prev+2)/2 */
            seq_t next = t * t;
            next = next - prev;
            cache[i] = next;
        }
        size = n;
    }

    return cache[n];
}

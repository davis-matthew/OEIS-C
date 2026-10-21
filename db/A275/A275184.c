/*
 * A275184  Number of 4 X n 0..2 arrays with no element equal to any value at
 *          offset (-2,-1) (-1,0) or (-1,1) and new values introduced in order
 *          0..2.
 * https://oeis.org/A275184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) + 3*a(n-3) for n>4 Initial values:
 *            a(1)=4, a(2)=12, a(3)=41, a(4)=141
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;   /* array of a[0..] (a[0] unused)   */
    static size_t cap = 0;        /* allocated capacity in elements   */
    static idx_t  computed = 0;   /* maximum index already computed   */

    if (n < 1)
        return 0;                 /* undefined for n<1; harmless     */

    /* Initialise cache with the initial four values on first call   */
    if (cache == NULL) {
        cap = 8;                          /* initial capacity (>=5)          */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[0] = 0;                     /* unused to keep indices 1‑based  */
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)12;
        cache[3] = (seq_t)41;
        cache[4] = (seq_t)141;
        computed = 4;
    }

    /* Expand allocation if n exceeds current capacity      */
    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while ((size_t)n >= new_cap) new_cap <<= 1;   /* double until ok */
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute missing terms up to n  (run only once per missing i)  */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* recurrence: a(i) = 2*a(i-1) + 4*a(i-2) + 3*a(i-3)   */
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t a3 = cache[i - 3];
        cache[i] = 2 * a1 + 4 * a2 + 3 * a3;
    }

    computed = (idx_t)((computed > n) ? computed : n);
    return cache[(size_t)n];
}

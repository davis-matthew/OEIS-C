/*
 * A248484  Number of length n+5 0..3 arrays with some three disjoint pairs in
 *          each consecutive six terms having the same sum.
 * https://oeis.org/A248484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 23, u32 n <= 71, u64 n <= 167, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 4*a(n-6) - 4*a(n-7)  for n >= 8
   Initial values (for the offset 1):
   a(1)=724, a(2)=844, a(3)=988, a(4)=1156, a(5)=1348,
   a(6)=1564, a(7)=1804 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Undefined for n == 0, return 0 for safety */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Convert index to size_t for array operations */
    size_t idx = (size_t)n;

    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap = 0;      /* allocated capacity of cache array   */
    static idx_t last = 0;      /* largest index whose value is known */

    /* Allocate or expand cache as necessary */
    if (cache == NULL) {                 /* first call */
        /* Allocate space for at least indices 0..7 */
        cap = (idx > 7 ? idx : 7) + 1;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;    /* allocation failed – return 0 */

        /* Initialise the first 7 values (index 0 unused) */
        cache[1] = (seq_t)724;
        cache[2] = (seq_t)844;
        cache[3] = (seq_t)988;
        cache[4] = (seq_t)1156;
        cache[5] = (seq_t)1348;
        cache[6] = (seq_t)1564;
        cache[7] = (seq_t)1804;

        last = 7;
    } else if (idx >= cap) {             /* need more space */
        size_t newcap = cap;
        while (idx >= newcap) newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* realloc failed – return 0 */
        cache = tmp;
        cap = newcap;
    }

    /* If value already computed, return it */
    if (idx <= (size_t)last) {
        return cache[idx];
    }

    /* Compute terms from last+1 up to idx using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* a(i) = a(i-1) + 4*a(i-6) - 4*a(i-7)  (valid for i >= 8) */
        cache[i] = cache[i - 1] + ((seq_t)4 * cache[i - 6])
                         - ((seq_t)4 * cache[i - 7]);
    }
    last = n;
    return cache[idx];
}

/*
 * A290890  p-INVERT of the positive integers, where p(S) = 1 - S^2.
 * https://oeis.org/A290890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + 4*a(n-3) - a(n-4) (n>=4) Initial
 *            values: a(0)=0, a(1)=1, a(2)=4, a(3)=11
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;   /* dynamic cache of computed terms   */
static size_t a_cap = 0;        /* allocated capacity of the cache   */
static idx_t a_used = 0;        /* number of terms that are initialized */

/*
 * Ensure that the cache can store terms up to index n inclusive.
 */
static void ensure_a_cache(idx_t n) {
    if (a_cap <= (size_t)n) {
        size_t new_cap = (size_t)(n + 1);
        if (new_cap < 16) new_cap = 16;    /* start with some room */
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        a_cache = tmp;
        a_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    /* Small indices have known constants */
    switch (n) {
    case 0: return (seq_t)0;
    case 1: return (seq_t)1;
    case 2: return (seq_t)4;
    case 3: return (seq_t)11;
    }

    /* For any n>=4 we use the recurrence.  Make sure the first four terms
       are present in the cache. */
    if (a_used < 4) {
        ensure_a_cache(3);
        a_cache[0] = (seq_t)0;
        a_cache[1] = (seq_t)1;
        a_cache[2] = (seq_t)4;
        a_cache[3] = (seq_t)11;
        a_used = 4;
    }

    /* Compute any missing terms up to n. */
    if (n >= a_used) {
        ensure_a_cache(n);
        for (idx_t i = a_used; i <= n; ++i) {
            a_cache[i] = 4 * a_cache[i - 1]
                         - 5 * a_cache[i - 2]
                         + 4 * a_cache[i - 3]
                         - a_cache[i - 4];
        }
        a_used = n + 1;
    }

    return a_cache[n];
}

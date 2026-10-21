/*
 * A251252  Number of (n+1) X (2+1) 0..1 arrays with every 2 X 2 subblock
 *          having a single 1 or two 1s on the same edge.
 * https://oeis.org/A251252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(1)=18, a(2)=56, a(3)=150, a(4)=446, a(5)=1232, a(n) =
 *            3*a(n-1) + 4*a(n-2) – 14*a(n-3) + 3*a(n-4) + 3*a(n-5) for n ≥ 6
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 40, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoised values.
       cache[0] is unused; cache[i] holds a(i) for i ≥ 1. */
    static seq_t *cache = NULL;          /* pointer to cache array   */
    static idx_t  max_cached = 0;        /* highest n that has been computed */
    static size_t capacity   = 0;        /* allocated size of cache   */

    /* Allocate or extend cache to size n+1 (index n included). */
    if (cache == NULL) {
        capacity = n + 1;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) return 0;    /* out of memory – unlikely in test harness */
    } else if (capacity <= n) {
        /* double the capacity to amortise reallocation cost */
        size_t new_capacity = (capacity < 64) ? 64 : capacity * 2;
        if (new_capacity <= n) new_capacity = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        if (tmp == NULL) return 0;     /* out of memory */
        cache = tmp;
        capacity = new_capacity;
    }

    /* Compute values up to n if not already computed. */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t val;
        switch (i) {
            case 1: val = (seq_t)18;     break;
            case 2: val = (seq_t)56;     break;
            case 3: val = (seq_t)150;    break;
            case 4: val = (seq_t)446;    break;
            case 5: val = (seq_t)1232;   break;
            default:
                val = 3 * cache[i - 1];
                val += 4 * cache[i - 2];
                val -= 14 * cache[i - 3];
                val += 3 * cache[i - 4];
                val += 3 * cache[i - 5];
                break;
        }
        cache[i] = val;
    }
    max_cached = n;
    return cache[n];
}

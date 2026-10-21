/*
 * A266930  Number of n X 3 binary arrays with rows lexicographically
 *          nondecreasing and columns lexicographically nondecreasing and row
 *          sums nondecreasing and column sums nonincreasing.
 * https://oeis.org/A266930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical linear recurrence (order 11) * a(1) = 2, a(2) = 4,
 *            a(3) = 9, a(4) = 20, a(5) = 44, * a(6) = 92, a(7) = 182, a(8) =
 *            340, a(9) = 605, a(10) = 1028, * a(11)= 1680 * For n ≥ 12: *
 *            a(n) = 4*a(n-1) - 4*a(n-2) - 3*a(n-3) + 6*a(n-4) * - 6*a(n-7) +
 *            3*a(n-8) + 4*a(n-9) - 4*a(n-10) + a(n-11)
 * Range:     u8 n <= 7, u16 n <= 21, u32 n <= 143, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External linkage as requested */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n==0 */

    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap = 0;       /* number of entries (>= n) */

    /* Initial values for first 11 terms */
    static const seq_t init[11] = {
        2ULL, 4ULL, 9ULL, 20ULL, 44ULL, 92ULL, 182ULL,
        340ULL, 605ULL, 1028ULL, 1680ULL
    };

    /* Allocate or grow cache as needed */
    if (cache == NULL || n > cap) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);        /* allocation failed */
        cache = tmp;
        /* initialize new entries */
        for (idx_t i = 0; i < n; ++i) {
            if (i < 11) {
                cache[i] = init[i];
            } else {
                /* compute a(i+1) using recurrence */
                seq_t val = 4 * cache[i - 1]
                          - 4 * cache[i - 2]
                          - 3 * cache[i - 3]
                          + 6 * cache[i - 4]
                          - 6 * cache[i - 7]
                          + 3 * cache[i - 8]
                          + 4 * cache[i - 9]
                          - 4 * cache[i -10]
                          +   cache[i -11];
                cache[i] = val;
            }
        }
        cap = new_cap;
    }

    return cache[n - 1];
}

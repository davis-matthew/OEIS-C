/*
 * A282371  Number of n X 2 0..1 arrays with no 1 equal to more than four of
 *          its king-move neighbors, with the exception of exactly two
 *          elements.
 * https://oeis.org/A282371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (empirical):
   a(1) = 0
   a(2) = 0
   a(3) = 1
   a(4) = 6
   a(5) = 33
   a(6) = 168
   a(n) = 6*a(n-1) - 3*a(n-2) - 12*a(n-3) - 27*a(n-4) -
          18*a(n-5) - 9*a(n-6)   for n >= 7
   See OEIS A282371.
*/
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                          /* offset is 1 */

    static seq_t *cache = NULL;            /* 1‑indexed array of terms */
    static size_t cap   = 0;               /* current allocated size   */
    static bool  init   = false;           /* have we stored base terms? */

    /* Ensure the cache can hold indices up to n */
    size_t needed = (size_t)n + 1;         /* index 0 unused */
    if (cap < needed) {
        size_t new_cap = cap == 0 ? 16 : cap * 2;
        while (new_cap < needed)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;                     /* allocation failure – unlikely */
        cache = tmp;
        /* zero newly allocated entries (not strictly needed for unsigned types) */
        for (size_t i = cap; i < new_cap; ++i)
            cache[i] = 0;
        cap = new_cap;
    }

    /* Store base case values once */
    if (!init) {
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 1;
        cache[4] = 6;
        cache[5] = 33;
        cache[6] = 168;
        init = true;
    }

    /* Compute terms up to n using the linear recurrence */
    for (size_t i = 7; i <= (size_t)n; ++i) {
        seq_t val = (seq_t)6 * cache[i - 1]
                  - (seq_t)3 * cache[i - 2]
                  - (seq_t)12 * cache[i - 3]
                  - (seq_t)27 * cache[i - 4]
                  - (seq_t)18 * cache[i - 5]
                  - (seq_t)9  * cache[i - 6];
        cache[i] = val;
    }

    return cache[(size_t)n];
}

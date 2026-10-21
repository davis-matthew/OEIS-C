/*
 * A283422  a(1) = a(2) = a(4) = 1; a(3) = 0; a(n) = a(n-a(n-1)-1) +
 *          a(n-a(n-2)-4) for n > 4.
 * https://oeis.org/A283422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = a(n-a(n-1)-1) + a(n-a(n-2)-4) with a(1)=a(2)=a(4)=1,
 *            a(3)=0
 * Range:     u8 n <= 486, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* compute the sequence value for any n >= 1 (0‑based or 1‑based
   index handled by using n as given).  Results are memoised in a
   static array so calls in any order are fast and memory efficient. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: 1‑based indices, stored as 0‑based array elements */
    static seq_t *cache = NULL;
    static size_t cap = 0;    /* allocated capacity of cache (elements)   */
    static size_t len = 0;    /* number of values currently cached         */

    /* ensure the cache is large enough */
    if ((size_t)n > cap) {
        size_t new_cap = (cap > 0) ? cap * 2 : 4;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* compute any missing terms up to n */
    for (size_t i = len + 1; i <= (size_t)n; ++i) {
        seq_t val;
        if (i == 1 || i == 2 || i == 4) {
            val = 1;
        } else if (i == 3) {
            val = 0;
        } else {
            seq_t a_nm1 = cache[i - 2];   /* a(i-1) */
            seq_t a_nm2 = cache[i - 3];   /* a(i-2) */
            idx_t  idx1 = (idx_t)(i - (idx_t)a_nm1 - 1);
            idx_t  idx2 = (idx_t)(i - (idx_t)a_nm2 - 4);
            seq_t  v1   = cache[idx1 - 1];
            seq_t  v2   = cache[idx2 - 1];
            val = v1 + v2;
        }
        cache[i - 1] = val;
    }
    len = (size_t)n;          /* all terms up to n are now cached   */
    return cache[n - 1];
}

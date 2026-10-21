/*
 * A269689  Number of length-n 0..7 arrays with no repeated value differing
 *          from the previous repeated value by plus or minus one modulo 7+1.
 * https://oeis.org/A269689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 14*a(n-1) - 43*a(n-2) - 42*a(n-3) with a(1)=8, a(2)=64,
 *            a(3)=512. The sequence is memoized in a static cache so that
 *            calls may appear in any order
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* no defined value for n==0 */

    /* static cache: cache[i] holds a(i+1) for i>=0 */
    static seq_t *cache = NULL;
    static size_t cap = 0;        /* current allocated capacity */
    static size_t len = 0;        /* number of terms computed so far */

    /* grow the cache if needed */
    if ((size_t)n > len)
    {
        /* allocate enough space for n terms */
        size_t needed = (size_t)n;
        if (cap < needed)
        {
            size_t newcap = cap ? cap * 2 : 16;
            while (newcap < needed) newcap <<= 1;
            seq_t *newcache = (seq_t *)malloc(newcap * sizeof(seq_t));
            if (newcache == NULL) return (seq_t)0;   /* allocation failed */
            if (cache)
            {
                memcpy(newcache, cache, len * sizeof(seq_t));
                free(cache);
            }
            cache = newcache;
            cap   = newcap;
        }

        /* initialise base cases if they have not yet been written */
        if (len == 0) { cache[0] = (seq_t)8;   len = 1; }
        if (len == 1 && n >= 2) { cache[1] = (seq_t)64; len = 2; }
        if (len == 2 && n >= 3) { cache[2] = (seq_t)512; len = 3; }

        /* compute missing terms using the recurrence */
        for (idx_t i = (idx_t)len + 1; i <= n; ++i)
        {
            /* indices: a(i-1) = cache[i-2], a(i-2) = cache[i-3], a(i-3) = cache[i-4] */
            seq_t a_nm1 = cache[i - 2];
            seq_t a_nm2 = cache[i - 3];
            seq_t a_nm3 = cache[i - 4];

            seq_t val = a_nm1 * (seq_t)14
                     - a_nm2 * (seq_t)43
                     - a_nm3 * (seq_t)42;

            cache[i - 1] = val;
        }
        len = (size_t)n;
    }

    return cache[n - 1];
}

/*
 * A261286  Number of (n+2)X(2+2) 0..1 arrays with each 3X3 subblock having
 *          clockwise perimeter pattern 00000001 00010101 or 01010101.
 * https://oeis.org/A261286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence for n > 17 * a(n) = a(n-1) +4*a(n-3)
 *            -a(n-4) -5*a(n-6) +2*a(n-8) +a(n-9) -a(n-10) * -3*a(n-11)
 *            +a(n-12)+a(n-13)+a(n-14) -2*a(n-15) -2*a(n-16) * * Initial
 *            values (for n = 1 … 17): * a(1) = 50, a(2) = 77, a(3) = 133,
 *            a(4) = 233, * a(5) = 409, a(6) = 743, a(7) = 1361, a(8) = 246...
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 31, u64 n <= 69, u128 n <= 143,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Undefined behaviour for n < 1; return 0 to allow testing */
    if (n == 0)
        return (seq_t)0;

    /* static cache and its size */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise the cache with the first 17 terms */
    if (cache == NULL)
    {
        cache_size = 18;              /* indices 0 … 17 (0 unused) */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL)            /* allocation failure – abort */
            return (seq_t)0;
        cache[0] = (seq_t)0;          /* unused placeholder */
        cache[1] = (seq_t)50;
        cache[2] = (seq_t)77;
        cache[3] = (seq_t)133;
        cache[4] = (seq_t)233;
        cache[5] = (seq_t)409;
        cache[6] = (seq_t)743;
        cache[7] = (seq_t)1361;
        cache[8] = (seq_t)2469;
        cache[9] = (seq_t)4485;
        cache[10] = (seq_t)8183;
        cache[11] = (seq_t)14901;
        cache[12] = (seq_t)27091;
        cache[13] = (seq_t)49293;
        cache[14] = (seq_t)89729;
        cache[15] = (seq_t)163277;
        cache[16] = (seq_t)297055;
        cache[17] = (seq_t)540549;
    }

    /* extend the cache if necessary */
    if (n >= cache_size)
    {
        idx_t old_size = cache_size;
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;        /* allocation failure – abort */
        cache = tmp;
        cache_size = new_size;

        /* compute missing terms using the recurrence */
        for (idx_t k = old_size; k <= n; ++k)
        {
            /* recurrence is only defined for k > 17; when k==old_size==18, all
               indices used are already present in the cache */
            seq_t pos = cache[k-1]
                       + (seq_t)4 * cache[k-3]
                       + (seq_t)2 * cache[k-8]
                       + cache[k-9]
                       + cache[k-12]
                       + cache[k-13]
                       + cache[k-14];
            seq_t neg = cache[k-4]
                       + (seq_t)5 * cache[k-6]
                       + cache[k-10]
                       + (seq_t)3 * cache[k-11]
                       + (seq_t)2 * cache[k-15]
                       + (seq_t)2 * cache[k-16];
            cache[k] = pos - neg;
        }
    }

    return cache[n];
}

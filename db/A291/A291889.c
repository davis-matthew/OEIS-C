/*
 * A291889  Number of symmetrically unique Dyck paths of semilength n and
 *          height five.
 * https://oeis.org/A291889
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 21, u64 n <= 40, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A291889: Number of symmetrically unique Dyck paths of semilength n
   and height five.  The sequence satisfies the linear recurrence

   a(n) = 10*a(n-1) - 33*a(n-2) + 20*a(n-3) + 113*a(n-4)
          - 234*a(n-5) + 66*a(n-6) + 224*a(n-7)
          - 240*a(n-8) + 84*a(n-9) - 9*a(n-10),

   for n ≥ 11.  The initial values are:
   a(0)=a(1)=a(2)=a(3)=a(4)=0,
   a(5)=1, a(6)=5, a(7)=29, a(8)=127, a(9)=540, a(10)=2117. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_n = 0;           /* current number of stored terms */

    if (n < 5)                     /* terms before the offset are not used */
        return 0;

    if (n + 1 > cache_n) {
        /* enlarge cache */
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return 0;              /* allocation failure: return 0 as a safe default */
        cache = tmp;
        cache_n = new_size;

        /* initialise stored terms */
        if (cache_n > 0) {
            /* values for indices 0..4 are zero */
            for (idx_t i = 0; i < 5 && i < cache_n; ++i)
                cache[i] = 0;

            /* known initial terms */
            static const seq_t init[11] = {
                0, /* a(0) */
                0, /* a(1) */
                0, /* a(2) */
                0, /* a(3) */
                0, /* a(4) */
                1, /* a(5) */
                5, /* a(6) */
                29,/* a(7) */
                127,/* a(8) */
                540,/* a(9) */
                2117/* a(10) */
            };
            for (idx_t i = 5; i < cache_n && i <= 10; ++i)
                cache[i] = init[i];

            /* compute further terms using the recurrence until n */
            for (idx_t i = 11; i < cache_n; ++i) {
                seq_t sum = 0;
                sum += (seq_t)10  * cache[i-1];
                sum -= (seq_t)33  * cache[i-2];
                sum += (seq_t)20  * cache[i-3];
                sum += (seq_t)113 * cache[i-4];
                sum -= (seq_t)234 * cache[i-5];
                sum += (seq_t)66  * cache[i-6];
                sum += (seq_t)224 * cache[i-7];
                sum -= (seq_t)240 * cache[i-8];
                sum += (seq_t)84  * cache[i-9];
                sum -= (seq_t)9   * cache[i-10];
                cache[i] = sum;
            }
        }
    }

    return cache[n];
}

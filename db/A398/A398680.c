/*
 * A398680  Number of spanning trees of the 3 X n grid graph up to
 *          automorphisms.
 * https://oeis.org/A398680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A398680: Number of spanning trees of the 3×n grid graph up to automorphisms
   Recurrence (valid for n > 17):
       a(n) = 18*a(n-1) - 63*a(n-2) - 144*a(n-3) + 1060*a(n-4)
              - 1296*a(n-5) - 1312*a(n-6) + 3492*a(n-7) - 1312*a(n-8)
              - 1296*a(n-9) + 1060*a(n-10) - 144*a(n-11) - 63*a(n-12)
              + 18*a(n-13) - a(n-14)
   Initial terms (n = 1 … 17) are hard‑coded from the OEIS b‑file.
*/

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_realloc.h"   /* seq_realloc: realloc suitable for seq_t types */

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache    = NULL;   /* array storing a[0 .. cache_len-1] */
    static idx_t cache_len = 0;      /* current allocated length */

    if (n == 0) return 0;            /* 0 is never requested but harmless */

    /* Grow the cache if necessary */
    if ((idx_t)n + 1 > cache_len) {
        idx_t old_len = cache_len;
        idx_t new_len = (idx_t)n + 1;          /* indices 0 .. n inclusive */

        cache = seq_realloc (cache, (size_t)old_len, (size_t)new_len);

        /* Initialise new elements to zero (default ctor for big ints) */
        for (idx_t i = old_len; i < new_len; ++i) {
            cache[i] = (seq_t)0;
        }

        /* Hard‑coded initial values for n = 1 … 17 */
        static const seq_t init[18] = {
            (seq_t)0ULL,   /* placeholder for index 0 */
            (seq_t)1ULL,
            (seq_t)6ULL,
            (seq_t)28ULL,
            (seq_t)629ULL,
            (seq_t)7650ULL,
            (seq_t)95328ULL,
            (seq_t)1193010ULL,
            (seq_t)14957693ULL,
            (seq_t)187592909ULL,
            (seq_t)2353035207ULL,
            (seq_t)29515498980ULL,
            (seq_t)370234151472ULL,
            (seq_t)4644121396005ULL,
            (seq_t)58254705813018ULL,
            (seq_t)730732663772775ULL,
            (seq_t)9166130936067241ULL,
            (seq_t)114977694821515542ULL   /* n = 17 */
        };
        for (idx_t i = 1; i < 18 && i < new_len; ++i) {
            cache[i] = init[i];
        }

        /* Extend the recurrence for n ≥ 18 */
        idx_t start = old_len > 18 ? old_len : 18;
        for (idx_t i = start; i <= n; ++i) {
            seq_t pos = (seq_t)18   * cache[i-1];
                     pos += (seq_t)1060  * cache[i-4];
                     pos += (seq_t)3492  * cache[i-7];
                     pos += (seq_t)1060  * cache[i-10];
                     pos += (seq_t)18    * cache[i-13];

            seq_t neg = (seq_t)63    * cache[i-2];
                     neg += (seq_t)144   * cache[i-3];
                     neg += (seq_t)1296  * cache[i-5];
                     neg += (seq_t)1312  * cache[i-6];
                     neg += (seq_t)1312  * cache[i-8];
                     neg += (seq_t)1296  * cache[i-9];
                     neg += (seq_t)144   * cache[i-11];
                     neg += (seq_t)63    * cache[i-12];
                     neg += (seq_t)1     * cache[i-14];

            cache[i] = pos - neg;
        }

        cache_len = new_len;
    }

    return cache[n];
}

/*
 * A269531  Number of length-n 0..2 arrays with no repeated value differing
 *          from the previous repeated value by other than one.
 * https://oeis.org/A269531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - a(n-2) - 10*a(n-3) + 6*a(n-4) + 4*a(n-5)
 *            Initial values (offset 1): a(1)=3, a(2)=9, a(3)=24, a(4)=64,
 *            a(5)=164
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* offset is 1, values below unused */

    static seq_t *cache = NULL;          /* memoized sequence values */
    static idx_t computed = 0;           /* number of elements in cache */

    if (computed < n) {
        /* Resize cache to hold 'n' elements */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp) {                      /* allocation failure */
            free(cache);
            cache = NULL;
            computed = 0;
            return 0;
        }
        cache = tmp;

        for (idx_t i = computed; i < n; ++i) {
            switch (i) {
                case 0: cache[i] = (seq_t)3;   break;   /* a(1) */
                case 1: cache[i] = (seq_t)9;   break;   /* a(2) */
                case 2: cache[i] = (seq_t)24;  break;   /* a(3) */
                case 3: cache[i] = (seq_t)64;  break;   /* a(4) */
                case 4: cache[i] = (seq_t)164; break;   /* a(5) */
                default:
                    /* recurrence: a(n) = 4*a(n-1) - a(n-2) - 10*a(n-3) + 6*a(n-4) + 4*a(n-5) */
                    seq_t t = (seq_t)4 * cache[i-1];
                    t -= cache[i-2];
                    t -= (seq_t)10 * cache[i-3];
                    t += (seq_t)6 * cache[i-4];
                    t += (seq_t)4 * cache[i-5];
                    cache[i] = t;
                    break;
            }
        }
        computed = n;
    }
    return cache[n-1];
}

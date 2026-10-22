/*
 * A317225  Number of nX3 0..1 arrays with every element unequal to 0, 1, 2,
 *          3, 6, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A317225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            3*a(n-1)+a(n-2)+7*a(n-3)-22*a(n-4)-23*a(n-5)+8*a(n-6)+29*a(n-7)+22*a(n-8)-20*a(n-9)-10*a(n-10)
 *            for n>12, with initial values a(1)=4, a(2)=21, a(3)=46,
 *            a(4)=150, a(5)=488, a(6)=1468, a(7)=4626, a(8)=14720,
 *            a(9)=45811, a(10)=143798, a(11)=453581, a(12)=1423094
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    static idx_t cached = 0;
    static seq_t *cache = NULL;

    /* initial values for indices 1..12 */
    static const seq_t init[13] = {
        (seq_t)0,
        (seq_t)4,
        (seq_t)21,
        (seq_t)46,
        (seq_t)150,
        (seq_t)488,
        (seq_t)1468,
        (seq_t)4626,
        (seq_t)14720,
        (seq_t)45811,
        (seq_t)143798,
        (seq_t)453581,
        (seq_t)1423094
    };

    if (n <= cached)
        return cache[n];

    /* create or enlarge the cache */
    seq_t *new_cache = seq_alloc((size_t)n + 1);   /* indices 0..n */

    if (cached == 0) {
        /* first call: initialise all needed values */
        for (idx_t i = 1; i <= n; ++i) {
            if (i <= 12) {
                new_cache[i] = init[i];
            } else {
                seq_t sum = (seq_t)3 * new_cache[i-1]
                          +  new_cache[i-2]
                          + (seq_t)7 * new_cache[i-3]
                          + (seq_t)8 * new_cache[i-6]
                          + (seq_t)29 * new_cache[i-7]
                          + (seq_t)22 * new_cache[i-8];
                seq_t sub = (seq_t)22 * new_cache[i-4]
                          + (seq_t)23 * new_cache[i-5]
                          + (seq_t)20 * new_cache[i-9]
                          + (seq_t)10 * new_cache[i-10];
                new_cache[i] = sum - sub;
            }
        }
        cache   = new_cache;
        cached  = n;
    } else {
        /* copy already computed values */
        for (idx_t i = 1; i <= cached; ++i)
            new_cache[i] = cache[i];

        /* compute only the new entries */
        for (idx_t i = cached + 1; i <= n; ++i) {
            if (i <= 12) {
                new_cache[i] = init[i];
            } else {
                seq_t sum = (seq_t)3 * new_cache[i-1]
                          +  new_cache[i-2]
                          + (seq_t)7 * new_cache[i-3]
                          + (seq_t)8 * new_cache[i-6]
                          + (seq_t)29 * new_cache[i-7]
                          + (seq_t)22 * new_cache[i-8];
                seq_t sub = (seq_t)22 * new_cache[i-4]
                          + (seq_t)23 * new_cache[i-5]
                          + (seq_t)20 * new_cache[i-9]
                          + (seq_t)10 * new_cache[i-10];
                new_cache[i] = sum - sub;
            }
        }
        cache   = new_cache;
        cached  = n;
    }

    return cache[n];
}

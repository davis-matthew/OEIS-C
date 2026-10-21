/*
 * A249467  Number of length 1+4 0..n arrays with no five consecutive terms
 *          having four times any element equal to the sum of the remaining
 *          four.
 * https://oeis.org/A249467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 5*a(n-3) - 4*a(n-4) + 2*a(n-5) +
 *            2*a(n-6) - 4*a(n-7) + 5*a(n-8) - 6*a(n-9) + 4*a(n-10) - a(n-11)
 *            initial values: a(1)=30, a(2)=190, a(3)=860, a(4)=2640,
 *            a(5)=6730, a(6)=14730, a(7)=29060, a(8)=52900, a(9)=90390,
 *            a(10)=146610, a(11)=228000
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 83, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle offset 1 */
    if (n == 0) return (seq_t)0;

    static seq_t *cache = NULL;          /* 0‑based array, cache[i] == a(i+1) */
    static size_t cache_cap = 0;         /* allocated capacity */
    static size_t cache_len = 0;         /* number of elements stored */

    /* enlarge cache if needed */
    if (cache_cap < (size_t)n) {
        size_t newcap = (cache_cap == 0) ? (size_t)n : cache_cap;
        if (newcap < (size_t)n) newcap = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* out of memory – unlikely */
        cache = tmp;
        cache_cap = newcap;
    }

    /* initialize initial values up to 11 if not yet */
    static const seq_t base[11] = {
        (seq_t)30,   (seq_t)190, (seq_t)860,  (seq_t)2640,
        (seq_t)6730, (seq_t)14730,(seq_t)29060,(seq_t)52900,
        (seq_t)90390,(seq_t)146610,(seq_t)228000
    };
    if (cache_len < 11) {
        size_t i;
        if (cache_len < 11) {
            for (i = cache_len; i < 11 && i < (size_t)n; ++i)
                cache[i] = base[i];
            cache_len = (cache_len < 11) ? (i > cache_len ? i : cache_len) : cache_len;
        }
    }

    /* compute missing terms up to n via recurrence */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        if (i < 11) { /* already set */
            continue;
        }
        /* i is 0‑based index, representing a(i+1) where (i+1) >=12 */
        seq_t v =  4 * cache[i-1]
                 - 6 * cache[i-2]
                 + 5 * cache[i-3]
                 - 4 * cache[i-4]
                 + 2 * cache[i-5]
                 + 2 * cache[i-6]
                 - 4 * cache[i-7]
                 + 5 * cache[i-8]
                 - 6 * cache[i-9]
                 + 4 * cache[i-10]
                 -    cache[i-11];
        cache[i] = v;
    }
    cache_len = (cache_len < (size_t)n) ? (size_t)n : cache_len;

    return cache[n-1];
}

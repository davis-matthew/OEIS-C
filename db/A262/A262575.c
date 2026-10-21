/*
 * A262575  Concatenation of the numbers from 1 to n but omitting 5.
 * https://oeis.org/A262575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n)=concatenation of the integers from 1 to n with the integer
 *            5 omitted
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   digits_of, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow10.h"
#include "../utils/digits_of.h"

seq_t a(idx_t n)
{
    /* static cache to support out‑of‑order calls */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;        /* largest n that has been computed */
    static idx_t cache_cap  = 0;        /* allocated capacity (>= cache_size+1) */

    if (n == 0)
        return 0;                      /* undefined for n=0 */

    if (n > cache_size) {
        /* ensure enough capacity */
        if (n >= cache_cap) {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 2;
            while (new_cap <= n)
                new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);         /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute terms up to n */
        seq_t prev = cache_size ? cache[cache_size] : 0;
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            idx_t m = (i < 5 ? i : i + 1);      /* integer to concatenate, 5 omitted */
            int d = digits_of(m);
            seq_t p = pow10(d);
            cache[i] = prev * p + (seq_t)m;
            prev = cache[i];
        }
        cache_size = n;
    }

    return cache[n];
}

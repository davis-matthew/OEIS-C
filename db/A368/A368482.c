/*
 * A368482  The degree of polynomials related to Somos-6 sequences. Also for n
 *          > 4 the degree of the (n-5)-th involution in a family of
 *          involutions in the Cremona group of rank 5 defined by a Somos-6
 *          sequence.
 * https://oeis.org/A368482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - a(n-4) + a(n-5) -
 *            2*a(n-6) + 2*a(n-7) - 2*a(n-8) + a(n-9); Initial values: a(0)=0,
 *            a(1)=0, a(2)=0, a(3)=0, a(4)=0, a(5)=0, a(6)=2, a(7)=3, a(8)=4.
 *            The recurrence applies for n >= 9
 * Range:     u8 n <= 43, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* ensure capacity */
    if (n >= cache_size) {
        idx_t new_size = cache_size > 0 ? cache_size : 10;
        while (new_size <= n)
            new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – abort with zero (should never happen) */
            exit(1);
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* fill cache up to n */
    for (idx_t i = 0; i <= n; ++i) {
        if (i < 9) {
            /* base values */
            switch (i) {
                case 0: case 1: case 2: case 3: case 4: case 5:
                    cache[i] = (seq_t)0;
                    break;
                case 6:
                    cache[i] = (seq_t)2;
                    break;
                case 7:
                    cache[i] = (seq_t)3;
                    break;
                case 8:
                    cache[i] = (seq_t)4;
                    break;
            }
        } else {
            /* recurrence with signed 64‑bit intermediate */
            long long t =
                2ll * (long long)cache[i - 1]
              - 2ll * (long long)cache[i - 2]
              + 2ll * (long long)cache[i - 3]
              - 1ll * (long long)cache[i - 4]
              + 1ll * (long long)cache[i - 5]
              - 2ll * (long long)cache[i - 6]
              + 2ll * (long long)cache[i - 7]
              - 2ll * (long long)cache[i - 8]
              + 1ll * (long long)cache[i - 9];
            cache[i] = (seq_t)t;
        }
    }

    return cache[n];
}

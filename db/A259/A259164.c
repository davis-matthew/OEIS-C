/*
 * A259164  Positive heptagonal numbers (A000566) that are squares (A000290)
 *          divided by 2.
 * https://oeis.org/A259164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*m_n^2, where (x_n,m_n) solves x_n^2-80*m_n^2=9 and x_n
 *            ≡ 7 (mod 10). Solutions are generated from the Pell recurrence
 *            using the minimal unit (9,1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t curr_x, curr_y;
    static bool initialized = false;

    if (!initialized) {
        /* first valid solution (x,y) = (27,3) */
        curr_x = (seq_t)27;
        curr_y = (seq_t)3;

        /* allocate cache for first term */
        cache_cap = 4;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        /* a(1) = 2*3^2 = 18 */
        seq_t t = curr_y * curr_y;
        t = t * (seq_t)2;
        cache[0] = t;
        cache_len = 1;
        initialized = true;
    }

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        /* next solution of Pell: (x',y') = (9*x + 80*y , x + 9*y) */
        seq_t x_next = curr_x * (seq_t)9 + curr_y * (seq_t)80;
        seq_t y_next = curr_x + curr_y * (seq_t)9;

        /* keep the term only if x_next ≡ 7 (mod 10) */
        if ((x_next % (seq_t)10) == (seq_t)7) {
            seq_t val = y_next * y_next;
            val = val * (seq_t)2; /* a = 2*y^2 */

            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 4;
                cache = (seq_t*)realloc(cache, sizeof(seq_t) * cache_cap);
            }
            cache[cache_len++] = val;
        }

        curr_x = x_next;
        curr_y = y_next;
    }

    return cache[n - 1];
}

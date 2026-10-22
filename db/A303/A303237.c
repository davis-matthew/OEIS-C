/*
 * A303237  Number of nX3 0..1 arrays with every element equal to 0, 2, 3, 5
 *          or 6 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A303237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) -5*a(n-2) +16*a(n-3) -36*a(n-4) +32*a(n-5)
 *            -63*a(n-6) + 64*a(n-7) -51*a(n-8) +4*a(n-9) +60*a(n-10)
 *            +16*a(n-11) +112*a(n-12) +64*a(n-13) initial values a(1)=1,
 *            a(2)=11, a(3)=18, a(4)=55, a(5)=181, a(6)=494, a(7)=1465,
 *            a(8)=4415, a(9)=12934, a(10)=38423, a(11)=114145,
 *            a(12)=337914,...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* undefined for n < 1 */

    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cap = 0;

    /* Grow cache if necessary */
    if (n >= cap) {
        size_t new_cap = (cap == 0) ? 16 : (cap * 2);
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = seq_realloc(cache, cap, new_cap);
        if (!tmp) return (seq_t)0;     /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialize the first 13 terms once */
    static int initialized = 0;
    if (!initialized) {
        static const seq_t init[14] = {
            (seq_t)0,  /* dummy for index 0 */
            (seq_t)1,  (seq_t)11,  (seq_t)18,  (seq_t)55,
            (seq_t)181, (seq_t)494, (seq_t)1465,
            (seq_t)4415, (seq_t)12934, (seq_t)38423,
            (seq_t)114145, (seq_t)337914, (seq_t)1003161
        };
        for (size_t i = 1; i <= 13; ++i) cache[i] = init[i];
        initialized = 1;
    }

    /* Return known values directly for n <= 13 */
    if (n <= 13) return cache[(size_t)n];

    /* Compute values up to n using the recurrence */
    for (idx_t i = 14; i <= n; ++i) {
        seq_t sum_pos = (seq_t)0;
        seq_t sum_neg = (seq_t)0;

        sum_pos += (seq_t)4   * cache[i-1];
        sum_pos += (seq_t)16  * cache[i-3];
        sum_pos += (seq_t)32  * cache[i-5];
        sum_pos += (seq_t)64  * cache[i-7];
        sum_pos += (seq_t)4   * cache[i-9];
        sum_pos += (seq_t)60  * cache[i-10];
        sum_pos += (seq_t)16  * cache[i-11];
        sum_pos += (seq_t)112 * cache[i-12];
        sum_pos += (seq_t)64  * cache[i-13];

        sum_neg += (seq_t)5   * cache[i-2];
        sum_neg += (seq_t)36  * cache[i-4];
        sum_neg += (seq_t)63  * cache[i-6];
        sum_neg += (seq_t)51  * cache[i-8];

        cache[i] = sum_pos - sum_neg;
    }

    return cache[(size_t)n];
}

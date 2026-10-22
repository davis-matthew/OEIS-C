/*
 * A361359  Number of nonequivalent noncrossing caterpillars with n edges up
 *          to rotation.
 * https://oeis.org/A361359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 3*a(n-2) - 26*a(n-3) + 36*a(n-4) * - 2*a(n-5)
 *            - 18*a(n-6) + 6*a(n-7) + 5*a(n-8) * - 4*a(n-9) + a(n-10) *
 *            a(0)=1, a(1)=1, a(2)=1, a(3)=4, a(4)=11, a(5)=49, * a(6)=196,
 *            a(7)=868, a(8)=3721, a(9)=16306
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 17, u64 n <= 32, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache --------------------------------------------------------- */
    static seq_t *cache = NULL;
    static size_t cached_len = 0;

    /* initialise base values (first ten terms of the sequence) */
    if (!cache) {
        cache = (seq_t*)malloc(10 * sizeof(seq_t));
        if (!cache) exit(1);                       /* allocation failure   */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)4;
        cache[4] = (seq_t)11;
        cache[5] = (seq_t)49;
        cache[6] = (seq_t)196;
        cache[7] = (seq_t)868;
        cache[8] = (seq_t)3721;
        cache[9] = (seq_t)16306;
        cached_len = 10;
    }

    /* if already computed, return it */
    if ((size_t)n < cached_len)
        return cache[n];

    /* extend cache up to n */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) exit(1);                     /* reallocation failure  */
    cache = tmp;

    /* compute missing terms using the recurrence */
    for (size_t i = cached_len; i < new_len; ++i) {
        seq_t t = (seq_t)0;
        t += (seq_t)6  * cache[i-1];
        t -= (seq_t)3  * cache[i-2];
        t -= (seq_t)26 * cache[i-3];
        t += (seq_t)36 * cache[i-4];
        t -= (seq_t)2  * cache[i-5];
        t -= (seq_t)18 * cache[i-6];
        t += (seq_t)6  * cache[i-7];
        t += (seq_t)5  * cache[i-8];
        t -= (seq_t)4  * cache[i-9];
        t +=          cache[i-10];
        cache[i] = t;
    }

    cached_len = new_len;
    return cache[(size_t)n];
}

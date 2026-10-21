/*
 * A252703  Number of strings of length n over a 10-letter alphabet that do
 *          not begin with a palindrome.
 * https://oeis.org/A252703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, walk
 * Formula:   a(n) = 10*a(n-1) - a(ceil(n/2)) (a(0)=0, a(1)=10)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of computed a(k) */
    static size_t cap   = 0;      /* allocated size of array    */
    static size_t len   = 0;      /* number of initialized values */

    /* enlarge array if needed */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap * 2 : 1;
        while (new_cap <= (size_t)n + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure – unlikely */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute values up to n */
    for (size_t i = len; i <= (size_t)n; ++i) {
        if (i == 0)
            cache[0] = (seq_t)0;
        else if (i == 1)
            cache[1] = (seq_t)10;
        else {
            seq_t val = (seq_t)10 * cache[i-1];
            seq_t sub = cache[(i + 1) / 2];   /* ceil(i/2) */
            cache[i] = val - sub;
        }
    }
    len = (size_t)n + 1;
    return cache[n];
}

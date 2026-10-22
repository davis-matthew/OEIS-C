/*
 * A302655  Number of minimal total dominating sets in the n-path graph.
 * https://oeis.org/A302655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3)+a(n-4)+a(n-5)+a(n-6)-a(n-8)-a(n-9); a(1)=0,
 *            a(2)=1, a(3)=2, a(4)=1, a(5)=2, a(6)=4, a(7)=3, a(8)=4, a(9)=8
 * Range:     u8 n <= 21, u16 n <= 41, u32 n <= 81, u64 n <= 160,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache – memoised values of the sequence */
    static seq_t *cache   = NULL;
    static idx_t capacity = 0;      /* allocated size (>= index 0)   */
    static idx_t max_comp = 0;      /* highest index with value set */

    /* Ensure the cache is large enough */
    if (n >= capacity) {
        idx_t new_cap = capacity ? capacity : 16;
        while (new_cap <= n) new_cap <<= 1;      /* grow geometrically */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                      /* allocation failure */
        cache = tmp;
        capacity = new_cap;
    }

    /* Fill the array up to n, using the recurrence or initial values */
    static const int base_int[10] = {0, 0, 1, 2, 1, 2, 4, 3, 4, 8};   /* indices 1..9 */

    for (idx_t i = max_comp + 1; i <= n; ++i) {
        if (i <= 9) {                       /* initial terms are known */
            cache[i] = (seq_t)base_int[i];
        } else {
            seq_t v = cache[i-3] + cache[i-4] + cache[i-5] + cache[i-6];
            v -= (cache[i-8] + cache[i-9]);
            cache[i] = v;
        }
    }
    max_comp = n;

    return cache[n];
}

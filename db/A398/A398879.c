/*
 * A398879  a(n) is the number of tilings of a 9 X 5n rectangle into
 *          I-pentominoes.
 * https://oeis.org/A398879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5),
 *            n>4 Initial values: a(0)=1, a(1)=6, a(2)=56, a(3)=506, a(4)=4556
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* lazily allocate and compute the sequence using the linear recurrence */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last = (idx_t)-1;

    if (cache == NULL) {
        cache_cap = 8;                         /* enough for first few terms */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)56;
        cache[3] = (seq_t)506;
        cache[4] = (seq_t)4556;
        last = 4;
    }

    /* enlarge cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap) {
            new_cap *= 2;
        }
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    while (last < n) {
        ++last;
        seq_t val = ((seq_t)10 * cache[last-1])
                    - ((seq_t)10 * cache[last-2])
                    + ((seq_t)10 * cache[last-3])
                    - ((seq_t)5  * cache[last-4])
                    + cache[last-5];
        cache[last] = val;
    }

    return cache[n];
}

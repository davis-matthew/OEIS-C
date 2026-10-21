/*
 * A264124  Number of (n+1)X(4+1) arrays of permutations of 0..n*5+4 with each
 *          element having index change +-(.,.) 0,0 1,-2 or 2,-1.
 * https://oeis.org/A264124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 19*a(n-1) + 16*a(n-2) - 304*a(n-3) + 5776*a(n-4) +
 *            4864*a(n-5) + 4096*a(n-6) - 77824*a(n-7) - 65536*a(n-8) for n >
 *            11. Initial values: a(1)=8, a(2)=200, a(3)=5733, a(4)=108045,
 *            a(5)=1995525, a(6)=39592980, a(7)=791859600, a(8)=15730470000,
 *            a(9)=311564912400, a(10)=6168985265520, a(11)=1221...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_terms[11] = {
    8ULL, 200ULL, 5733ULL, 108045ULL, 1995525ULL,
    39592980ULL, 791859600ULL,
    15730470000ULL, 311564912400ULL,
    6168985265520ULL, 122179379691600ULL
};

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* value below the offset is irrelevant */

    /* dynamic cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t last_computed = 0; /* maximum n that has been computed so far */
    static size_t capacity = 0;     /* allocated size of cache */

    /* ensure cache is large enough */
    if (n > capacity) {
        size_t new_cap = capacity ? capacity : 1;
        while (new_cap < n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        capacity = new_cap;
    }

    /* compute missing terms sequentially */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        if (i <= 11) {
            cache[i-1] = init_terms[i-1];
        } else { /* recurrence for n > 11 */
            seq_t term = 19 * cache[i-2];
            term += 16 * cache[i-3];
            term += 5776 * cache[i-5];
            term += 4864 * cache[i-6];
            term += 4096 * cache[i-7];
            term -= 304 * cache[i-4];
            term -= 77824 * cache[i-8];
            term -= 65536 * cache[i-9];
            cache[i-1] = term;
        }
    }

    last_computed = n;
    return cache[n-1];
}

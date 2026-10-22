/*
 * A303315  Number of 3Xn 0..1 arrays with every element equal to 0, 1, 4, 5
 *          or 6 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A303315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-3) + 4*a(n-4) Initial values: a(1)=4,
 *            a(2)=4, a(3)=12, a(4)=17, a(5)=24, a(6)=67, a(7)=158
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 59, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed = 0;   /* largest index whose value is cached */

    const size_t INITIAL_CAP = 16;   /* start with a few slots */

    /* Allocate cache on first call */
    if (!cache) {
        cache_size = INITIAL_CAP;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return (seq_t)0;   /* allocation failed – unlikely in tests */
    }

    /* Grow cache if needed for index n */
    if ((idx_t)cache_size <= n) {
        size_t new_size = cache_size * 2;
        if (new_size <= (size_t)n) new_size = (size_t)n + 8;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (tmp) {
            cache = tmp;
            cache_size = new_size;
        } else {
            return (seq_t)0;   /* realloc failed – unlikely in tests */
        }
    }

    /* Initialise the first seven terms once */
    if (computed < 7) {
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)12;
        cache[4] = (seq_t)17;
        cache[5] = (seq_t)24;
        cache[6] = (seq_t)67;
        cache[7] = (seq_t)158;
        computed = 7;
    }

    /* Return cached value if already computed */
    if (n <= computed) {
        return cache[n];
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t val = cache[i - 1];
        val += (seq_t)3 * cache[i - 3];
        val += (seq_t)4 * cache[i - 4];
        cache[i] = val;
    }
    computed = n;
    return cache[n];
}

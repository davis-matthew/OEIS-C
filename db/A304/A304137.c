/*
 * A304137  Number of nX4 0..1 arrays with every element unequal to 0, 2, 3 or
 *          6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-3) - 6*a(n-4) + 3*a(n-6) (n > 10)
 *            Initial values (n = 1..10): a(1)=1 a(2)=13 a(3)=9 a(4)=11
 *            a(5)=20 a(6)=17 a(7)=25 a(8)=62 a(9)=59 a(10)=93
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Base values for n = 1..10 (index 0 is unused) */
static const seq_t BASE[11] = {0, 1, 13, 9, 11, 20, 17, 25, 62, 59, 93};

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;        /* dynamic cache of computed values */
    static size_t capacity = 0;        /* allocated size of cache */
    static idx_t computed = 0;         /* highest n whose value is computed */

    /* Ensure the cache is large enough to store a[n] */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();     /* out of memory */
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute values up to n if necessary */
    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            if (i <= 10) {
                cache[i] = BASE[i];
            } else {
                /* recurrence: a(i) = 2*a(i-1) + 2*a(i-3) - 6*a(i-4) + 3*a(i-6) */
                seq_t val = 2 * cache[i-1];
                val += 2 * cache[i-3];
                val -= 6 * cache[i-4];
                val += 3 * cache[i-6];
                cache[i] = val;
            }
        }
        computed = n;
    }

    return cache[n];
}

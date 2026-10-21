/*
 * A298777  Number of nX3 0..1 arrays with every element equal to 0, 3, 4, 5
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 48, u64 n <= 94, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=1, a(3)=6, a(4)=2, a(5)=4, a(6)=7, a(7)=8, a(8)=15;
   a(n)=a(n-1)+a(n-2) for all n>8 */

#include <stdlib.h>

/* Base values for the first 8 terms */
static const seq_t base_values[9] = {
    (seq_t)0,   /* dummy for index 0 */
    (seq_t)1,   /* a(1) */
    (seq_t)1,   /* a(2) */
    (seq_t)6,   /* a(3) */
    (seq_t)2,   /* a(4) */
    (seq_t)4,   /* a(5) */
    (seq_t)7,   /* a(6) */
    (seq_t)8,   /* a(7) */
    (seq_t)15   /* a(8) */
};

/* Memoization cache */
static seq_t *cache = NULL;      /* cache[1..last] valid */
static idx_t  last = 0;          /* highest index with a computed value */
static idx_t  capacity = 0;      /* allocated size (>= last+1) */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;          /* behaviour for offset <1 does not matter */
    }

    /* Expand cache if necessary */
    if (capacity < n + 1) {
        /* Choose new capacity: at least n+1 and at least double the old size */
        idx_t new_capacity = capacity * 2;
        if (new_capacity < n + 1) new_capacity = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        if (tmp == NULL) {
            return (seq_t)0;      /* allocation failed – unlikely in tests */
        }
        cache = tmp;
        capacity = new_capacity;
    }

    /* Fill missing values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i <= 8) {
            cache[i] = base_values[i];
        } else {
            cache[i] = cache[i - 1] + cache[i - 2];
        }
    }
    last = n;

    return cache[n];
}

/*
 * A390688  a(n) = Sum_{k=0..floor(2*n/9)} binomial(k+1,2*n-9*k).
 * https://oeis.org/A390688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 84, u32 n <= 161, u64 n <= 314,
 *            u128 n <= 618, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula   : a(n) = Σ_{k=0..⌊2n/9⌋} C(k+1, 2n-9k)
 * Recurrence: a(n) = 2*a(n-5) + a(n-9) - a(n-10)   for n ≥ 10
 * Initial    : a(0)=1, a(1)=0, a(2)=0, a(3)=0, a(4)=0,
 *              a(5)=2, a(6)=0, a(7)=0, a(8)=0, a(9)=1
 */

#include <stdlib.h>

static const seq_t init_vals[10] = {
    1, 0, 0, 0, 0,
    2, 0, 0, 0, 1
};

seq_t a(idx_t n)
{
    /* cache of previously computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;    /* allocated size */
    static size_t size  = 0;    /* next index to compute (cache[0..size-1] are valid) */

    /* enlarge cache if necessary */
    if ((idx_t)size <= n) {
        size_t newcap = (cap == 0) ? 16 : cap;
        while (newcap <= (size_t)n) newcap <<= 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* compute missing values up to n */
    for (size_t i = size; i <= (size_t)n; ++i) {
        seq_t val;
        if (i < 10) {
            val = init_vals[i];              /* initial terms */
        } else {
            /* recurrence uses already computed earlier indices */
            seq_t a5  = cache[i - 5];
            seq_t a9  = cache[i - 9];
            seq_t a10 = cache[i - 10];
            val = 2 * a5 + a9 - a10;
        }
        cache[i] = val;
    }
    size = (size_t)n + 1;   /* next index to compute will be n+1 */

    return cache[(size_t)n];
}

/*
 * A387843  a(n) = Sum_{k=0..n} binomial(2*n,3*k).
 * https://oeis.org/A387843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*n,3*k) Recurrence: a(0)=1,
 *            a(1)=1, a(2)=5, a(n)=3*a(n-1)+3*a(n-2)+4*a(n-3) for n>2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t cache[10001];   /* enough for n up to 10000 */
static idx_t cached_size = 0;

seq_t a(idx_t n)
{
    /* Extend the cache only up to n – order of calls does not matter */
    if (n + 1 > cached_size) {
        idx_t old = cached_size;

        /* Initialise base values that are still undefined */
        if (old == 0) {
            if (n >= 0) { cache[0] = (seq_t)1; old = 1; }
        }
        if (old == 1) {
            if (n >= 1) { cache[1] = (seq_t)1; old = 2; }
        }
        if (old == 2) {
            if (n >= 2) { cache[2] = (seq_t)5; old = 3; }
        }

        /* Compute remaining entries using the recurrence */
        for (idx_t i = old; i <= n; ++i) {
            /* For i < 3 the values are already set above */
            if (i >= 3) {
                cache[i] = (seq_t)3 * cache[i-1]
                         + (seq_t)3 * cache[i-2]
                         + (seq_t)4 * cache[i-3];
            }
        }
        cached_size = n + 1;
    }
    return cache[n];
}

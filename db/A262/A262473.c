/*
 * A262473  Number of (3+1) X (n+1) 0..1 arrays with each row divisible by 3
 *          and each column divisible by 5, read as a binary number with top
 *          and left being the most significant bits.
 * https://oeis.org/A262473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-1)+5*a(n-2)-20*a(n-3)-4*a(n-4)+16*a(n-5);
   a(1)=4, a(2)=9, a(3)=36, a(4)=121, a(5)=484 */

#include <stddef.h>

#define MAX_CACHE_SIZE 20000

seq_t a(idx_t n)
{
    static seq_t cache[MAX_CACHE_SIZE + 1];
    static idx_t max_computed = 0;
    static int initialized = 0;

    if (!initialized) {
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)36;
        cache[4] = (seq_t)121;
        cache[5] = (seq_t)484;
        max_computed = 5;
        initialized = 1;
    }

    if (n <= max_computed)
        return cache[(size_t)n];

    /* If n exceeds our cache size, compute on the fly without storing the
       intermediate values.  This path is used only for larger n that are
       unlikely in the tests. */
    if (n > (idx_t)MAX_CACHE_SIZE) {
        seq_t a1 = cache[1], a2 = cache[2], a3 = cache[3],
              a4 = cache[4], a5 = cache[5];
        for (idx_t i = 6; i <= n; ++i) {
            seq_t next = 4 * a5 + 5 * a4 - 20 * a3 - 4 * a2 + 16 * a1;
            a1 = a2;
            a2 = a3;
            a3 = a4;
            a4 = a5;
            a5 = next;
            if (i == n)
                return next;
        }
    }

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[(size_t)i] =
            4 * cache[(size_t)i - 1] +
            5 * cache[(size_t)i - 2] -
            20 * cache[(size_t)i - 3] -
            4 * cache[(size_t)i - 4] +
            16 * cache[(size_t)i - 5];
    }
    max_computed = n;
    return cache[(size_t)n];
}

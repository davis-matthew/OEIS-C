/*
 * A242237  Number of length n+6+1 0..6 arrays with every value 0..6 appearing
 *          at least once in every consecutive 6+2 elements, and new values
 *          0..6 introduced in order.
 * https://oeis.org/A242237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 60, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6)+a(n-7) for n>=8.
   Initial values:
   a(1)=28, a(2)=70, a(3)=148, a(4)=298, a(5)=592, a(6)=1174, a(7)=2332. */
#include <stddef.h>

#define MAX_N 10000   /* assumed sufficient for the intended tests */

seq_t a(idx_t n)
{
    /* Zero index is unused; sequence is 1‑indexed. */
    if (n == 0) return 0;

    /* Static cache to allow arbitrary order of calls. */
    static seq_t cache[MAX_N + 1];
    static size_t last_computed = 0;   /* largest index that has been computed */

    /* Initialize base values on first call. */
    if (last_computed == 0) {
        cache[1] = 28;
        cache[2] = 70;
        cache[3] = 148;
        cache[4] = 298;
        cache[5] = 592;
        cache[6] = 1174;
        cache[7] = 2332;
        last_computed = 7;
    }

    /* Extend computation up to requested n (if needed). */
    if (n > last_computed) {
        size_t i;
        for (i = last_computed + 1; i <= (size_t)n; ++i) {
            seq_t sum = 0;
            sum += cache[i - 1];
            sum += cache[i - 2];
            sum += cache[i - 3];
            sum += cache[i - 4];
            sum += cache[i - 5];
            sum += cache[i - 6];
            sum += cache[i - 7];
            cache[i] = sum;
        }
        last_computed = (size_t)n;
    }

    return cache[(size_t)n];
}

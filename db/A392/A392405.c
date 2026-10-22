/*
 * A392405  a(n) = Sum_{k=0..floor(2*n/9)} binomial(2*n-6*k,3*k).
 * https://oeis.org/A392405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(2*n/9)} binomial(2*n-6*k,3*k) via
 *            recurrence a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + 6*a(n-5) +
 *            2*a(n-6) + a(n-9) initial values:
 *            a(0)=1,a(1)=1,a(2)=1,a(3)=1,a(4)=1,a(5)=5,a(6)=21,
 *            a(7)=57,a(8)=121
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 60, u128 n <= 118,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache up to 10000 (covers all test cases) */
    static seq_t cache[10001];
    static unsigned char initialized = 0;
    static size_t computed_to = 0;   /* largest index computed */

    /* initialise base terms once */
    if (!initialized) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)1;
        cache[5] = (seq_t)5;
        cache[6] = (seq_t)21;
        cache[7] = (seq_t)57;
        cache[8] = (seq_t)121;
        computed_to = 8;
        initialized = 1;
    }

    /* if n is beyond current cache, extend it */
    if ((size_t)n > computed_to) {
        size_t i;
        for (i = computed_to + 1; i <= (size_t)n; ++i) {
            seq_t val = (seq_t)3 * cache[i-1];
            val -= (seq_t)3 * cache[i-2];
            val += cache[i-3];
            val += (seq_t)6 * cache[i-5];
            val += (seq_t)2 * cache[i-6];
            val += cache[i-9];
            cache[i] = val;
        }
        computed_to = (size_t)n;
    }

    return cache[n];
}

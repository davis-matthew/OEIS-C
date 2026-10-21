/*
 * A259214  Number of (n+1)X(n+1) 0..1 arrays with each 2X2 subblock having
 *          clockwise pattern 0000 0011 or 0101.
 * https://oeis.org/A259214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) -10*a(n-2) -2*a(n-3) +16*a(n-4) -6*a(n-5)
 *            -5*a(n-6) +2*a(n-7); Initial values: a(1)=7, a(2)=23, a(3)=66,
 *            a(4)=183, a(5)=487, a(6)=1278, a(7)=3319;
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t cache[10001];
    static idx_t computed = 0;

    if (n == 0) return 0;            /* not used, but guard against NULL index */

    /* initialise the first 7 terms once */
    if (computed == 0) {
        cache[1] = (seq_t)7;
        cache[2] = (seq_t)23;
        cache[3] = (seq_t)66;
        cache[4] = (seq_t)183;
        cache[5] = (seq_t)487;
        cache[6] = (seq_t)1278;
        cache[7] = (seq_t)3319;
        computed = 7;
    }

    /* compute further terms if needed */
    if (computed < n) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t term1 = 6 * cache[i - 1] + 16 * cache[i - 4] + 2 * cache[i - 7];
            seq_t term2 = 10 * cache[i - 2] + 2 * cache[i - 3] + 6 * cache[i - 5] + 5 * cache[i - 6];
            cache[i] = term1 - term2;
        }
        computed = n;
    }

    return cache[n];
}

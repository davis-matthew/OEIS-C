/*
 * A260171  Number of (n+2)X(2+2) 0..1 arrays with each 3X3 subblock having
 *          clockwise perimeter pattern 00000001 00000101 or 00010001.
 * https://oeis.org/A260171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+5*a(n-3)+5*a(n-4)-3*a(n-5)+5*a(n-6)
 *            +9*a(n-7)-5*a(n-8)-5*a(n-9)+4*a(n-10)-17*a(n-11)
 *            -14*a(n-12)+4*a(n-13)-4*a(n-14)-4*a(n-15) for n>16 Initial
 *            values:
 *            a(1)=74,a(2)=167,a(3)=419,a(4)=1001,a(5)=2500,a(6)=6344,a(7)=15847,
 *            a(8)=39584,a(9)=99741,a(10)=250297,a(11)=626433,a(12)=15...
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 20, u64 n <= 44, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    static seq_t *cache    = NULL;
    static idx_t  cached   = 0;   /* highest index already computed */
    static size_t capacity = 0;   /* allocated array size */

    /* allocate or grow buffer as needed */
    if (n > cached) {
        size_t needed = (size_t)n + 1; /* indices 0..n inclusive */
        if (needed > capacity) {
            seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE); /* allocation failed */
            cache    = tmp;
            capacity = needed;
            cache[0] = 0; /* sentinel for index 0 */
        }

        /* initial values a(1)..a(16) */
        static const seq_t init[17] = {
            0ULL, /* unused at index 0 */
            74ULL,    167ULL,    419ULL,   1001ULL,
            2500ULL,  6344ULL,   15847ULL, 39584ULL,
            99741ULL, 250297ULL, 626433ULL, 1572166ULL,
            3946872ULL, 9894451ULL, 24814854ULL, 62266809ULL
        };

        for (idx_t i = cached + 1; i <= n; ++i) {
            if (i <= 16) {
                cache[i] = init[i];
            } else {
                seq_t val = 0;
                val += cache[i-1];
                val += cache[i-2];
                val += (seq_t)5 * cache[i-3];
                val += (seq_t)5 * cache[i-4];
                val -= (seq_t)3 * cache[i-5];
                val += (seq_t)5 * cache[i-6];
                val += (seq_t)9 * cache[i-7];
                val -= (seq_t)5 * cache[i-8];
                val -= (seq_t)5 * cache[i-9];
                val += (seq_t)4 * cache[i-10];
                val -= (seq_t)17 * cache[i-11];
                val -= (seq_t)14 * cache[i-12];
                val += (seq_t)4 * cache[i-13];
                val -= (seq_t)4 * cache[i-14];
                val -= (seq_t)4 * cache[i-15];
                cache[i] = val;
            }
        }
        cached = n;
    }

    return cache[n];
}

/*
 * A302261  Number of nX4 0..1 arrays with every element equal to 0, 1, 2 or 3
 *          horizontally, diagonally or antidiagonally adjacent elements, with
 *          upper left element zero.
 * https://oeis.org/A302261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) -2*a(n-2) -51*a(n-3) -80*a(n-4) -11*a(n-5) *
 *            +262*a(n-6) +1203*a(n-7) -73*a(n-8) -847*a(n-9) -3807*a(n-10) *
 *            +103*a(n-11) -606*a(n-12) +4295*a(n-13) +1436*a(n-14)
 *            +1630*a(n-15) * -3052*a(n-16) +576*a(n-17) for n>18; initial
 *            values * a(1)=8, a(2)=105, a(3)=772, a(4)=5922, a(5)=4571...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t _init[18] = {
    8ULL,
    105ULL,
    772ULL,
    5922ULL,
    45717ULL,
    353229ULL,
    2727755ULL,
    21069318ULL,
    162753849ULL,
    1257225302ULL,
    9711792270ULL,
    75021745843ULL,
    579529894725ULL,
    4476770505137ULL,
    34582309009590ULL,
    267142643360807ULL,
    2063633101348406ULL,
    15941227812038514ULL
};

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset is 1 – undefined for 0 */

    static seq_t *cache = NULL;
    static idx_t cached = 0; /* number of terms already computed */

    if (n > cached) {
        size_t newsize = (size_t)n;
        cache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        for (idx_t i = cached; i < n; ++i) {
            if (i < 18) {            /* initial known terms */
                cache[i] = _init[i];
            } else {                 /* recurrence for n = i+1 */
                seq_t pos = 0;
                seq_t neg = 0;

                pos += 9ULL   * cache[i - 1];
                pos += 262ULL * cache[i - 6];
                pos += 1203ULL* cache[i - 7];
                pos += 103ULL * cache[i - 11];
                pos += 4295ULL* cache[i - 13];
                pos += 1436ULL* cache[i - 14];
                pos += 1630ULL* cache[i - 15];
                pos += 576ULL * cache[i - 17];

                neg += 2ULL   * cache[i - 2];
                neg += 51ULL  * cache[i - 3];
                neg += 80ULL  * cache[i - 4];
                neg += 11ULL  * cache[i - 5];
                neg += 73ULL  * cache[i - 8];
                neg += 847ULL * cache[i - 9];
                neg += 3807ULL* cache[i - 10];
                neg += 606ULL * cache[i - 12];
                neg += 3052ULL* cache[i - 16];

                cache[i] = pos - neg;
            }
        }
        cached = n;
    }

    return cache[n - 1];
}

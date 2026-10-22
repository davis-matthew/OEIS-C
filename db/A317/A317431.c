/*
 * A317431  Number of nX3 0..1 arrays with every element unequal to 1, 2, 3,
 *          5, 6, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A317431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence for A317431 (valid for n > 21):
   a(n) = 3*a(n-1) + 5*a(n-2) + 10*a(n-3) 
          - 29*a(n-4) - 69*a(n-5) - 36*a(n-6) 
          + 68*a(n-7) + 201*a(n-8) + 115*a(n-9) + 35*a(n-10)
          - 218*a(n-11) - 192*a(n-12) - 83*a(n-13)
          + 2*a(n-14) + 113*a(n-15) + 61*a(n-16) + 18*a(n-17) + 4*a(n-18)
          - 10*a(n-19) - 4*a(n-20);
   Initial values (n = 1 … 21):
     1, 16, 61, 201, 863, 3666, 14831, 61638, 255772, 1057307,
     4380003, 18147665, 75156794, 311315739, 1289593418,
     5341744958, 22126877953, 91655931185, 379663396847,
     1572669053056, 6514430280073 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* cache[0] is unused; indices 1..max_computed hold computed terms */
    static seq_t cache[10001];
    static idx_t max_computed = 0;

    /* initialise base values once */
    if (max_computed == 0)
    {
        cache[0] = 0;
        cache[1] = 1ULL;
        cache[2] = 16ULL;
        cache[3] = 61ULL;
        cache[4] = 201ULL;
        cache[5] = 863ULL;
        cache[6] = 3666ULL;
        cache[7] = 14831ULL;
        cache[8] = 61638ULL;
        cache[9] = 255772ULL;
        cache[10] = 1057307ULL;
        cache[11] = 4380003ULL;
        cache[12] = 18147665ULL;
        cache[13] = 75156794ULL;
        cache[14] = 311315739ULL;
        cache[15] = 1289593418ULL;
        cache[16] = 5341744958ULL;
        cache[17] = 22126877953ULL;
        cache[18] = 91655931185ULL;
        cache[19] = 379663396847ULL;
        cache[20] = 1572669053056ULL;
        cache[21] = 6514430280073ULL;
        max_computed = 21;
    }

    if (n <= max_computed)
        return cache[n];

    /* compute missing terms up to n, using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        seq_t pos = cache[i-1] * 3
                  + cache[i-2] * 5
                  + cache[i-3] * 10
                  + cache[i-7] * 68
                  + cache[i-8] * 201
                  + cache[i-9] * 115
                  + cache[i-10] * 35
                  + cache[i-14] * 2
                  + cache[i-15] * 113
                  + cache[i-16] * 61
                  + cache[i-17] * 18
                  + cache[i-18] * 4;

        seq_t neg = cache[i-4] * 29
                  + cache[i-5] * 69
                  + cache[i-6] * 36
                  + cache[i-11] * 218
                  + cache[i-12] * 192
                  + cache[i-13] * 83
                  + cache[i-19] * 10
                  + cache[i-20] * 4;

        cache[i] = pos - neg;
    }

    max_computed = n;
    return cache[n];
}

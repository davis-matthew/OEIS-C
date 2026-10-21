/*
 * A264579  Number of (n+1) X (4+1) arrays of permutations of 0..n*5+4 with
 *          each element having directed index change 1,0 0,-1 1,2 or -1,1.
 * https://oeis.org/A264579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=10, a(3)=28, a(4)=104, a(5)=505, a(6)=2144,
 *            a(7)=8185, a(n) = 2*a(n-1) + 36*a(n-3) + 9*a(n-4) - 243*a(n-6)
 *            for n>7
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>


/* 10000 is sufficient for the required range (n up to about 10000). */
static seq_t cache[10001];
static bool initialized = false;
static idx_t max_cached = 0;

seq_t a(idx_t n)
{
    /* support a(1) .. a(10000); larger n will cause an out‑of‑bounds access */
    if (n == 0) return 0;                 /* 0 is never requested in OEIS offset 1 */

    if (!initialized) {
        /* store the first seven known values */
        cache[1] = 1;
        cache[2] = 10;
        cache[3] = 28;
        cache[4] = 104;
        cache[5] = 505;
        cache[6] = 2144;
        cache[7] = 8185;
        max_cached = 7;
        initialized = true;
    }

    if (n > max_cached) {
        /* extend computation only up to the requested n */
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            if (i <= 7) {
                /* already set above, but keep code explicit for safety */
                switch (i) {
                    case 1: cache[i] = 1; break;
                    case 2: cache[i] = 10; break;
                    case 3: cache[i] = 28; break;
                    case 4: cache[i] = 104; break;
                    case 5: cache[i] = 505; break;
                    case 6: cache[i] = 2144; break;
                    case 7: cache[i] = 8185; break;
                }
            } else {
                seq_t t1 = cache[i-1] * 2;
                seq_t t2 = cache[i-3] * 36;
                seq_t t3 = cache[i-4] * 9;
                seq_t t4 = cache[i-6] * 243;
                cache[i] = t1 + t2 + t3 - t4;
            }
        }
        max_cached = n;
    }

    return cache[n];
}

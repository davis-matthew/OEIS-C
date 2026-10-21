/*
 * A299130  Number of nX3 0..1 arrays with every element equal to 0, 2, 3, 4,
 *          5 or 7 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A299130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A299130: number of n×3 binary arrays with king‑move
   adjacency constraints and upper‑left element zero.
   Recurrence (n > 14):
   a(n) = 5*a(n-1) + 7*a(n-2) - 27*a(n-3) - 65*a(n-4)
           + 97*a(n-5) + 118*a(n-6) - 276*a(n-7)
           + 110*a(n-8) + 516*a(n-9) - 226*a(n-10)
           - 372*a(n-11) - 12*a(n-12)
   Initial values:
   a(1)=1, a(2)=13, a(3)=30, a(4)=149, a(5)=576,
   a(6)=2621, a(7)=12495, a(8)=59426, a(9)=291819,
   a(10)=1434777, a(11)=7089514, a(12)=35139759,
   a(13)=174262279, a(14)=865022922.
*/

#include <stdbool.h>

/* maximum n we will handle – enough for n≈10000 */
#define MAX_N (20000)

static seq_t cache[MAX_N];
static bool computed[MAX_N];
static bool init_done = false;

/* Compute a(n) with memoisation */
seq_t a(idx_t n)
{
    if (n < 1 || n >= MAX_N)
        return 0;                       /* out of supported range */

    if (!init_done) {                    /* initialise first 14 values */
        cache[1]  = 1;   computed[1]  = true;
        cache[2]  = 13;  computed[2]  = true;
        cache[3]  = 30;  computed[3]  = true;
        cache[4]  = 149; computed[4]  = true;
        cache[5]  = 576; computed[5]  = true;
        cache[6]  = 2621; computed[6]  = true;
        cache[7]  = 12495; computed[7]  = true;
        cache[8]  = 59426; computed[8]  = true;
        cache[9]  = 291819; computed[9]  = true;
        cache[10] = 1434777; computed[10] = true;
        cache[11] = 7089514; computed[11] = true;
        cache[12] = 35139759; computed[12] = true;
        cache[13] = 174262279; computed[13] = true;
        cache[14] = 865022922; computed[14] = true;
        init_done = true;
    }

    /* Compute any missing values up to n (O(n) overall) */
    for (idx_t i = 1; i <= n; ++i) {
        if (!computed[i]) {
            /* recurrence valid for i > 14 */
            cache[i] =  5 * cache[i-1]
                      + 7 * cache[i-2]
                      - 27 * cache[i-3]
                      - 65 * cache[i-4]
                      + 97 * cache[i-5]
                      + 118 * cache[i-6]
                      - 276 * cache[i-7]
                      + 110 * cache[i-8]
                      + 516 * cache[i-9]
                      - 226 * cache[i-10]
                      - 372 * cache[i-11]
                      - 12 * cache[i-12];
            computed[i] = true;
        }
    }

    return cache[n];
}

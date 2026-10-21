/*
 * A258538  Number of (n+2) X (1+2) 0..1 arrays with no 3 x 3 subblock
 *          diagonal sum less than the antidiagonal sum or central row sum
 *          equal to the central column sum.
 * https://oeis.org/A258538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 32*a(n-3) + 34*a(n-4) + 54*a(n-5) * -
 *            96*a(n-6) + 63*a(n-8) - 27*a(n-9) * with initial values
 *            a(1)=220, a(2)=784, a(3)=2520, a(4)=8100, * a(5)=25056,
 *            a(6)=77284, a(7)=234848, a(8)=712336, a(9)=2148108
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 15, u64 n <= 36, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Maximum index handled by the static cache.  The sequence grows slowly,
 * and the tests require n up to ~10000, so 100000 is ample. */
#define MAX_N 100000

/* Static cache of computed terms.  Index 0 is unused. */
static seq_t cache[MAX_N + 1];
static bool computed[MAX_N + 1] = {0};

/* Base values for n = 1 … 9.  Index 0 is unused. */
static const seq_t base_vals[10] = {
    0ULL,            /* unused */
    220ULL, 784ULL, 2520ULL, 8100ULL, 25056ULL,
    77284ULL, 234848ULL, 712336ULL, 2148108ULL
};

seq_t a(idx_t n)
{
    /* For indices larger than the static cache we fall back to a
     * simple iterative computation that does not use the cache. */
    if (n > MAX_N) {
        if (n <= 9) {
            return base_vals[n];
        }
        /* Compute using a rotating buffer of the last nine terms. */
        seq_t buf[9] = {0};
        for (idx_t i = 1; i <= 9; ++i)
            buf[i-1] = base_vals[i];
        for (idx_t i = 10; ; ++i) {
            seq_t term = 0;
            term += 5 * buf[8];
            term -= 32 * buf[6];
            term += 34 * buf[5];
            term += 54 * buf[4];
            term -= 96 * buf[3];
            term += 63 * buf[1];
            term -= 27 * buf[0];
            /* Shift the buffer */
            for (int k = 0; k < 8; ++k) buf[k] = buf[k+1];
            buf[8] = term;
            if (i == n) return term;
        }
        /* Should never reach here */
        return 0;
    }

    /* Compute all terms up to n using the cache. */
    for (idx_t i = 1; i <= n; ++i) {
        if (!computed[i]) {
            if (i <= 9) {
                cache[i] = base_vals[i];
            } else {
                seq_t term = 0;
                term += 5 * cache[i-1];
                term -= 32 * cache[i-3];
                term += 34 * cache[i-4];
                term += 54 * cache[i-5];
                term -= 96 * cache[i-6];
                term += 63 * cache[i-8];
                term -= 27 * cache[i-9];
                cache[i] = term;
            }
            computed[i] = true;
        }
    }
    return cache[n];
}

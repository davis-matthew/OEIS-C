/*
 * A338551  Number of ways to make a checkout score of n in darts.
 * https://oeis.org/A338551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = number of sequences of at most 3 darts that sum to n,
 *            where the last dart is a double (2×i for 1≤i≤20 or 50)
 * Range:     u8 n <= 19, u16 n <= 170, u32 n <= 170, u64 n <= 170,
 *            u128 n <= 170, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements seq_t a(idx_t n) with the described behavior. */
#include <stdbool.h>

static bool initialized = false;
static seq_t ways1[171];
static seq_t ways2[171];

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined, but safe */
    if (n > 170) return 0;        /* no checkout beyond 170 */

    if (!initialized) {
        /* initialize ways1 array */
        for (idx_t i = 1; i <= 20; ++i) {
            ways1[i] += (seq_t)1;          /* single i */
            ways1[2*i] += (seq_t)1;        /* double i */
            ways1[3*i] += (seq_t)1;        /* triple i */
        }
        ways1[25] += (seq_t)1;  /* single bull */
        ways1[50] += (seq_t)1;  /* double bull */

        /* compute ways2 as convolution of ways1 with itself
           (ordered pairs of two darts) */
        for (idx_t v0 = 0; v0 <= 170; ++v0) {
            if (ways1[v0] == (seq_t)0) continue;
            for (idx_t v1 = 0; v1 <= 170; ++v1) {
                if (ways1[v1] == (seq_t)0) continue;
                idx_t sum = v0 + v1;
                if (sum <= 170) {
                    ways2[sum] += ways1[v0] * ways1[v1];
                }
            }
        }
        initialized = true;
    }

    seq_t result = 0;
    idx_t nn = n;

    /* double values from 2 to 40 (i=1..20) */
    for (idx_t i = 1; i <= 20; ++i) {
        idx_t d = 2 * i;
        if (d > nn) break;
        idx_t t = nn - d;          /* score needed from preceding darts */

        if (t == 0) result += (seq_t)1;  /* one dart only */
        result += ways1[t];                /* one preceding dart */
        result += ways2[t];                /* two preceding darts */
    }

    /* double bull (50) */
    if (50 <= nn) {
        idx_t t = nn - 50;
        if (t == 0) result += (seq_t)1;
        result += ways1[t];
        result += ways2[t];
    }

    return result;
}

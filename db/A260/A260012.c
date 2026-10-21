/*
 * A260012  Number of (n+2)X(5+2) 0..1 arrays with each 3X3 subblock having
 *          clockwise perimeter pattern 00000101 00010101 or 01010101.
 * https://oeis.org/A260012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-2) + 108*a(n-4) - 288*a(n-6) - 1828*a(n-8) +
 *            4768*a(n-10) + 4128*a(n-12) - 11008*a(n-14) - 4608*a(n-16) +
 *            4096*a(n-18) for n > 21 Initial terms: a(1)=436, a(2)=1232,
 *            a(3)=6944, a(4)=25600, a(5)=87024, a(6)=294336, a(7)=1231648,
 *            a(8)=4380288, a(9)=16773440, a(10)=58243968, a(11)=23170...
 * Range:     u8 none, u16 n <= 4, u32 n <= 13, u64 n <= 30, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[22] = {
    0ULL,                          /* 0 unused */
    436ULL,                        /* 1 */
    1232ULL,                       /* 2 */
    6944ULL,                       /* 3 */
    25600ULL,                      /* 4 */
    87024ULL,                      /* 5 */
    294336ULL,                     /* 6 */
    1231648ULL,                    /* 7 */
    4380288ULL,                    /* 8 */
    16773440ULL,                   /* 9 */
    58243968ULL,                   /* 10 */
    231701952ULL,                  /* 11 */
    814232832ULL,                  /* 12 */
    3185356416ULL,                 /* 13 */
    11129195264ULL,                /* 14 */
    43863022976ULL,                /* 15 */
    153683692032ULL,               /* 16 */
    603684726016ULL,               /* 17 */
    2112267329024ULL,              /* 18 */
    8309822447360ULL,              /* 19 */
    29094787617792ULL,             /* 20 */
    114381212785152ULL             /* 21 */
};

static seq_t *dp = NULL;
static idx_t  dp_size = 0;

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Allocate or expand storage */
    if (!dp) {
        size_t init_cap = 22;
        dp = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (!dp) abort();
        dp[0] = (seq_t)0;
        for (idx_t i = 1; i < init_cap; ++i) dp[i] = init_vals[i];
        dp_size = init_cap;
    }
    if (n + 1 > dp_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (!tmp) abort();
        dp = tmp;
        /* Compute new entries */
        for (idx_t i = dp_size; i < new_size; ++i) {
            if (i <= 21) {
                dp[i] = init_vals[i];
                continue;
            }
            seq_t val = (seq_t)8 * dp[i-2] + (seq_t)108 * dp[i-4];
            val -= (seq_t)288 * dp[i-6];
            val -= (seq_t)1828 * dp[i-8];
            val += (seq_t)4768 * dp[i-10];
            val += (seq_t)4128 * dp[i-12];
            val -= (seq_t)11008 * dp[i-14];
            val -= (seq_t)4608 * dp[i-16];
            val += (seq_t)4096 * dp[i-18];
            dp[i] = val;
        }
        dp_size = new_size;
    }
    return dp[n];
}

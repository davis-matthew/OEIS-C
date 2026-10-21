/*
 * A252264  Number of (n+2) X (3+2) 0..3 arrays with every 3 X 3 subblock row,
 *          column, diagonal and antidiagonal sum not equal to 2 3 4 6 or 7.
 * https://oeis.org/A252264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=258, a(2)=822, a(3)=3016, a(4)=10302, a(5)=35486,
 *            a(6)=122506, a(7)=420420, a(8)=1446744, a(9)=4990438,
 *            a(10)=17183040, a(11)=59122262, a(12)=203560110,
 *            a(13)=701041422, a(14)=2413836036, a(15)=8310468310,
 *            a(16)=28613117804, a(17)=98520290558, and for n>17
 *            a(n)=a(n‑1)+a(n‑2)+12*a(n‑3)+32*a(n...
 * Range:     u8 none, u16 n <= 5, u32 n <= 14, u64 n <= 32, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* ------------------------------------------------------------ */
/*  Storage for the computed sequence                          */
/* ------------------------------------------------------------ */
static seq_t *dp            = NULL;   /* array of values, dp[0] unused       */
static size_t dp_size       = 0;      /* number of indices currently stored  */
static size_t dp_capacity   = 0;      /* allocated capacity                   */

static const seq_t init_vals[18] = {
    0, /* dp[0] unused */
    258ULL, 822ULL, 3016ULL, 10302ULL, 35486ULL,
    122506ULL, 420420ULL, 1446744ULL, 4990438ULL,
    17183040ULL, 59122262ULL, 203560110ULL, 701041422ULL,
    2413836036ULL, 8310468310ULL, 28613117804ULL, 98520290558ULL
};

/* Ensure the array has capacity for at least 'need' indices (indices 0..need-1). */
static void reserve_capacity(size_t need)
{
    if (dp_capacity >= need)
        return;
    size_t newcap = dp_capacity ? dp_capacity : 32;
    while (newcap < need)
        newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(dp, newcap * sizeof(seq_t));
    dp      = tmp;
    dp_capacity = newcap;
}

/* Initialise the first 17 terms of the sequence. */
static void init_sequence(void)
{
    if (dp_size > 0)
        return; /* already initialised */

    reserve_capacity(18);               /* enough for indices 0..17   */
    for (idx_t i = 1; i <= 17; ++i)
        dp[i] = init_vals[i];
    dp_size = 18; /* highest stored index + 1 */
}

/* Compute terms up to index 'n' (inclusive). */
static void compute_to(idx_t n)
{
    if (n + 1 <= dp_size)   /* already computed */
        return;

    reserve_capacity((size_t)n + 1);

    for (idx_t i = (idx_t)dp_size; i <= n; ++i) {
        if (i <= 17) {          /* should already be set, but keep for safety */
            dp[i] = init_vals[i];
        } else {
            seq_t val =
                dp[i - 1] + dp[i - 2] + (seq_t)12 * dp[i - 3]
                + (seq_t)32 * dp[i - 4] + (seq_t)44 * dp[i - 5]
                + (seq_t)37 * dp[i - 6] - (seq_t)32 * dp[i - 7]
                - (seq_t)81 * dp[i - 8] - (seq_t)12 * dp[i - 9]
                + (seq_t)47 * dp[i -10] + (seq_t)38 * dp[i -11]
                + (seq_t)8  * dp[i -12]
                - (seq_t)6  * dp[i -13] - (seq_t)4 * dp[i -14]
                - (seq_t)4  * dp[i -15];
            dp[i] = val;
        }
    }
    dp_size = (size_t)n + 1;
}

/* ------------------------------------------------------------ */
/*  Public function requested by the problem statement          */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for n<1, but safe */

    init_sequence();
    compute_to(n);
    return dp[n];
}

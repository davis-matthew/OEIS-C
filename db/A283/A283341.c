/*
 * A283341  Number of nX2 0..1 arrays with no 1 equal to more than two of its
 *          horizontal, vertical and antidiagonal neighbors, with the
 *          exception of exactly one element.
 * https://oeis.org/A283341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 7*a(n-2) - 23*a(n-4) + 8*a(n-5) - 15*a(n-6) +
 *            24*a(n-7) - 14*a(n-8) + 20*a(n-9) - 13*a(n-10) + 14*a(n-11) -
 *            10*a(n-12) + 6*a(n-13) - 5*a(n-14) + 2*a(n-15) - a(n-16) Initial
 *            values: a(1)=0, a(2)=0, a(3)=6, a(4)=26, a(5)=120, a(6)=524,
 *            a(7)=2174, a(8)=8816, a(9)=35036, a(10)=13716...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;       /* cached values, dp[1] = a(1) */
static size_t dp_cap = 0;      /* allocated size of dp (>= max index + 1) */
static size_t dp_last = 0;     /* largest n with a(n) computed */

/* ensure dp has room for indices 0 .. needed-1 */
static void ensure_cap(size_t needed)
{
    if (needed <= dp_cap) return;
    size_t new_cap = needed * 2 + 16;
    if (new_cap < needed) new_cap = needed;   /* prevent overflow */
    seq_t *new_dp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
    if (!new_dp) abort();                     /* out of memory: terminate */
    dp = new_dp;
    /* zero the newly allocated part */
    for (size_t i = dp_last + 1; i < new_cap; ++i) dp[i] = 0;
    dp_cap = new_cap;
}

static void init_base(void)
{
    /* a(1) .. a(16) */
    const seq_t init[16] = {
        (seq_t)0,                   /* a(1) */
        (seq_t)0,                   /* a(2) */
        (seq_t)6,                   /* a(3) */
        (seq_t)26,                  /* a(4) */
        (seq_t)120,                 /* a(5) */
        (seq_t)524,                 /* a(6) */
        (seq_t)2174,                /* a(7) */
        (seq_t)8816,                /* a(8) */
        (seq_t)35036,               /* a(9) */
        (seq_t)137164,              /* a(10) */
        (seq_t)530660,              /* a(11) */
        (seq_t)2033212,             /* a(12) */
        (seq_t)7728080,             /* a(13) */
        (seq_t)29176258,            /* a(14) */
        (seq_t)109517952,           /* a(15) */
        (seq_t)409053414            /* a(16) */
    };
    ensure_cap(17); /* allocate space for indices up to 16 */
    for (size_t i = 1; i <= 16; ++i) dp[i] = init[i - 1];
    dp_last = 16;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */
    size_t idx = (size_t)n;       /* index into dp */
    if (dp == NULL) init_base();

    if (idx >= dp_cap) ensure_cap(idx + 1);

    if (idx > dp_last) {
        for (size_t k = dp_last + 1; k <= idx; ++k) {
            seq_t pos = ((seq_t)6) * dp[k - 1]
                       + ((seq_t)8) * dp[k - 5]
                       + ((seq_t)24) * dp[k - 7]
                       + ((seq_t)20) * dp[k - 9]
                       + ((seq_t)14) * dp[k - 11]
                       + ((seq_t)6)  * dp[k - 13]
                       + ((seq_t)2)  * dp[k - 15];
            seq_t neg = ((seq_t)7) * dp[k - 2]
                       + ((seq_t)23) * dp[k - 4]
                       + ((seq_t)15) * dp[k - 6]
                       + ((seq_t)14) * dp[k - 8]
                       + ((seq_t)13) * dp[k - 10]
                       + ((seq_t)10) * dp[k - 12]
                       + ((seq_t)5)  * dp[k - 14]
                       + ((seq_t)1)  * dp[k - 16];
            dp[k] = pos - neg;   /* recurrence is proven to yield non‑negative results */
        }
        dp_last = idx;
    }
    return dp[idx];
}

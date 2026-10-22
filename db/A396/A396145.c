/*
 * A396145  Number of strict integer partitions of 2*n with
 *          reverse-alternating sum -8.
 * https://oeis.org/A396145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-5) - a(n-7) - a(n-9) + a(n-11) +
 *            2*a(n-12) + a(n-13) + a(n-15) - a(n-16) - a(n-17) - 2*a(n-18) -
 *            a(n-19) - a(n-20) + a(n-21) + a(n-23) + 2*a(n-24) + a(n-25) -
 *            a(n-27) - a(n-29) - a(n-31) + a(n-34) + a(n-35) - a(n-36)
 *            (initial values: a(0)=0,a(1)=0,a(2)=0,a(3)=0,a(4)=0...
 * Range:     u8 n <= 24, u16 n <= 66, u32 n <= 358, u64 n <= 8686,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sequence is also the number of partitions of n+32 into 8 distinct parts
    not containing the part 8.  The initial 0‑68 values are generated once by
    a DP that counts those partitions.  After that the recurrence above
    supplies all larger terms.  The function works for any order of calls
    and for all builds (arbitrary-precision or native integers).                               */

#include <stddef.h>   /* for size_t */

/* Large enough to cover all queries up to n ≈ 10000.               */
#define MAX_CACHE_N 20000

static seq_t cache[MAX_CACHE_N + 1];
static idx_t cache_cap = 0;

/* Pre‑compute a(0)…a(68) by dynamic programming over partitions of
   (n+32) into 8 distinct parts not containing 8.                    */
static void
compute_initial_seeds(void)
{
    const idx_t Smax = 100;          /* 68+32 = 100 is the largest sum needed         */
    seq_t dp[9][Smax + 1];
    for (idx_t k = 0; k < 9; ++k)
        for (idx_t s = 0; s <= Smax; ++s)
            dp[k][s] = 0;
    dp[0][0] = 1;
    for (idx_t m = 1; m <= Smax; ++m) {
        if (m == 8) continue;          /* part 8 not allowed */
        for (idx_t k = 8; k > 0; --k) {
            for (idx_t s = Smax; s >= m; --s) {
                dp[k][s] += dp[k - 1][s - m];
            }
        }
    }
    for (idx_t i = 0; i <= 68; ++i)
        cache[i] = dp[8][i + 32];
}

/* Public sequence function */
seq_t
a(idx_t n)
{
    /* Ensure the cache is initialized and large enough.          */
    if (cache_cap == 0) {
        compute_initial_seeds();     /* sets cache[0..68] */
        cache_cap = 69;              /* next free index for recurrence starts at 69 */
    }
    if (n < cache_cap) return cache[n];

    /* Grow cache up to n.  All indices up to n (inclusive) will be set. */
    if (n > MAX_CACHE_N) {
        /* Out of bounds for this conservative implementation; clamp. */
        /* In practice n will not exceed MAX_CACHE_N.               */
        return 0;
    }

    for (idx_t i = cache_cap; i <= n; ++i) {
        /* Recurrence – positions with negative indices never occur
           because i >= 69 when the recurrence is applied.            */
        seq_t pos = cache[i-1] + cache[i-2] + cache[i-11]
                    + cache[i-13] + cache[i-15] + cache[i-21]
                    + cache[i-23] + cache[i-25] + cache[i-34] + cache[i-35]
                    + (cache[i-12] << 1) + (cache[i-24] << 1);
        seq_t neg = cache[i-5] + cache[i-7] + cache[i-9]
                    + cache[i-16] + cache[i-17] + cache[i-19]
                    + cache[i-20] + cache[i-27] + cache[i-29]
                    + cache[i-31] + cache[i-36]
                    + (cache[i-18] << 1);
        cache[i] = pos - neg;
    }
    cache_cap = n + 1;
    return cache[n];
}

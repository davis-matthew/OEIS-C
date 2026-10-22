/*
 * A317624  Number of integer partitions of n where all parts are > 1 and
 *          whose LCM is n.
 * https://oeis.org/A317624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of integer partitions of n into parts > 1 whose LCM
 *            equals n
 * Range:     u8 n <= 59, u16 n <= 119, u32 n <= 335, u64 n <= 359,
 *            u128 n <= 359, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include <math.h>

/* Assuming seq_t and idx_t are defined by the build. */
extern seq_t gcd(seq_t a, seq_t b); /* we may use from utils, but local gcd below */

static seq_t local_gcd(seq_t a, seq_t b)
{
    while (b) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    /* 1. Collect divisors of n (including 1), sorted ascending. */
    idx_t divs[256];
    size_t dsize = 0;
    idx_t lim = (idx_t)floor(sqrt((double)n));
    for (idx_t i = 1; i <= lim; ++i) {
        if (n % i == 0) {
            divs[dsize++] = i;
            if (i != n / i)
                divs[dsize++] = n / i;
        }
    }
    /* sorting */
    for (size_t i = 0; i < dsize; ++i)
        for (size_t j = i + 1; j < dsize; ++j)
            if (divs[i] > divs[j]) {
                idx_t t = divs[i];
                divs[i] = divs[j];
                divs[j] = t;
            }

    /* Find index of 1 and n */
    size_t idx1 = 0, idxn = dsize - 1;
    /* 2. Precompute lcm index table */
    size_t lcm_idx[256][256];
    for (size_t i = 0; i < dsize; ++i) {
        seq_t vi = (seq_t)divs[i];
        for (size_t j = 0; j < dsize; ++j) {
            seq_t vj = (seq_t)divs[j];
            seq_t g = local_gcd((seq_t)vi, (seq_t)vj);
            seq_t l = (vi / g) * vj; /* lcm(vi,vj) <= n */
            /* find index of l */
            size_t k = 0;
            while (divs[k] != l) ++k; /* linear search, dsize small */
            lcm_idx[i][j] = k;
        }
    }

    /* 3. Allocate DP table */
    size_t total = (size_t)(n + 1) * dsize;
    seq_t *dp = (seq_t *)calloc(total, sizeof(seq_t));
    if (!dp) return 0; /* allocation failure */

    dp[0 * dsize + idx1] = 1; /* base case: sum 0 with LCM 1 */

    /* 4. DP over parts (divisors >1) */
    for (size_t p = 1; p < dsize; ++p) { /* skip divisor 1 as part */
        idx_t val = divs[p];
        if (val > n) continue;
        for (idx_t s = val; s <= n; ++s) {
            size_t base = (size_t)(s - val) * dsize;
            size_t cur = (size_t)s * dsize;
            for (size_t prev = 0; prev < dsize; ++prev) {
                seq_t cnt = dp[base + prev];
                if (!cnt) continue;
                size_t newidx = lcm_idx[prev][p];
                dp[cur + newidx] += cnt;
            }
        }
    }

    seq_t res = dp[(size_t)n * dsize + idxn];
    free(dp);
    return res;
}

/*
 * A244825  Number of length n 0..1 arrays with each partial sum starting from
 *          the beginning no more than two standard deviations from its mean.
 * https://oeis.org/A244825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts binary strings of length n whose every partial sum
 *            S_k satisfies |S_k - k/2| <= sqrt(k). This is equivalent to
 *            (2*S_k - k)^2 <= 4*k for each k = 1..n. The function implements
 *            an O(n^2) dynamic‑programming algorithm that keeps, for each
 *            length k, the number of valid strings ending with...
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


/* External linkage required by the build scripts */
seq_t a(idx_t n)
{
    /* n >= 1 by the OEIS specification; for safety return 0 for n==0 */
    if (n == 0) return 0;

    /* Allocate two DP rows of length n+1, type seq_t (arbitrary‑precision
       or unsigned integer, as supplied by the build).  The extra slot
       simplifies the bounds handling. */
    size_t sz = (size_t)(n + 1);
    seq_t *dp   = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *dp_n = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp || !dp_n) {
        /* Allocation failure: avoid crashing the caller.  The sequence
           probably never hits this path in normal use. */
        free(dp); free(dp_n);
        return 0;
    }

    /* Initial state: length 0, sum 0 - one empty string. */
    dp[0] = 1;

    /* Iterate over lengths 1..n.  At step 'len', dp holds counts for
       sequences of length len-1; dp_n will receive the new values. */
    for (idx_t len = 1; len <= n; ++len) {
        /* Bounds for the new partial sum s at this length. */
        /* Lower bound: ceil(len/2 - sqrt(len)) */
        /* Upper bound: floor(len/2 + sqrt(len)) */
        /* We test each possible previous sum directly against the
           inequality (2*s - len)^2 <= 4*len, which is equivalent. */

        /* Zero the new row up to len (only these indices are used). */
        for (idx_t s = 0; s <= len; ++s)
            dp_n[s] = 0;

        /* Transition from all valid sums for len-1 to len. */
        for (idx_t s_prev = 0; s_prev <= len - 1; ++s_prev) {
            seq_t cnt = dp[s_prev];
            if (cnt == 0) continue;

            /* Append a 0: new sum = s_prev */
            long long diff0 = (long long)2 * (long long)s_prev - (long long)len;
            if (diff0 * diff0 <= (long long)4 * (long long)len)
                dp_n[s_prev] += cnt;

            /* Append a 1: new sum = s_prev + 1 */
            long long diff1 = (long long)2 * (long long)(s_prev + 1) - (long long)len;
            if (diff1 * diff1 <= (long long)4 * (long long)len)
                dp_n[s_prev + 1] += cnt;
        }

        /* Prepare for next iteration: swap pointers.  No allocations
           happen in the inner loop, so this is O(1). */
        seq_t *tmp = dp;
        dp = dp_n;
        dp_n = tmp;
    }

    /* Sum all counts for length n. */
    seq_t total = 0;
    for (idx_t s = 0; s <= n; ++s)
        total += dp[s];

    /* Clean up. */
    free(dp);
    free(dp_n);

    return total;
}

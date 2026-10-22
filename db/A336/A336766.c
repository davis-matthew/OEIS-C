/*
 * A336766  The number of partitions of n into an even number of parts, each
 *          part occurring at most five times, minus the number of partitions
 *          of n into an odd number of parts, each part occurring at most five
 *          times.
 * https://oeis.org/A336766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of q^n in ∏_{k>0} ((1 - q^(6k))/(1 + q^k))
 * Range:     i8 n <= 71, i16 n <= 71, i32 n <= 71, i64 n <= 71, i128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A336766.
   The sequence is the difference between the number of partitions of n
   into an even number of parts each occurring at most five times and
   those into an odd number of parts (still at most five times). */

#include <stdlib.h>
#include <string.h>

/* clarification: seq_t and idx_t types are supplied by the build environment. */
seq_t a(idx_t n) {
    /* Allocate two temporary arrays of size n+1.  We use calloc to
       initialise them to zero. */
    size_t sz = (size_t)n + 1;
    seq_t *dp = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *tmp = (seq_t*)calloc(sz, sizeof(seq_t));
    if (!dp || !tmp) {
        /* In case of allocation failure, abort the program. */
        abort();
    }

    /* Base case: empty partition has weight +1. */
    dp[0] = (seq_t)1;

    /* Precompute the signs for multiplicity 0..5 of a part. */
    const seq_t sign[6] = { (seq_t)1, (seq_t)-1, (seq_t)1, (seq_t)-1,
                            (seq_t)1, (seq_t)-1 };

    /* Process each part size i from 1 to n.  The generating function
       factor for part i is Σ_{k=0}^{5} (-1)^k q^{k*i}.  We perform
       a convolution of the current polynomial `dp` with this
       factor. */
    for (idx_t i = 1; i <= n; ++i) {
        /* tmp already zeroed from previous iteration. */
        for (idx_t s = 0; s <= n; ++s) {
            seq_t val = dp[s];
            if (val == 0)
                continue;

            /* Add contributions for using part i zero to five times. */
            for (int k = 0; k <= 5; ++k) {
                idx_t t = s + (idx_t)k * i;
                if (t > n)
                    break;
                tmp[t] += sign[k] * val;
            }
        }

        /* Swap dp and tmp for the next iteration.  Clear tmp to zero. */
        seq_t *o = dp;
        dp = tmp;
        tmp = o;
        memset(tmp, 0, sz * sizeof(seq_t));
    }

    /* Result is the coefficient for q^n. */
    seq_t result = dp[n];

    /* Clean up. */
    free(dp);
    free(tmp);

    return result;
}

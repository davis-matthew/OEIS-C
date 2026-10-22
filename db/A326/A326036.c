/*
 * A326036  Number of uniform complete integer partitions of n.
 * https://oeis.org/A326036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count uniform complete integer partitions of n (sequence A326036) */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Recursive helper: count ways to write rem as a sum of distinct
   integers > last, each not exceeding bound = m*pref + 1. */
static seq_t dfs_count(idx_t rem, idx_t pref, idx_t last, idx_t m)
{
    if (rem == 0) {
        return (seq_t)1;
    }
    seq_t total = (seq_t)0;
    idx_t bound = m * pref + (idx_t)1;
    if (bound > rem) {
        bound = rem;
    }
    for (idx_t v = last + (idx_t)1; v <= bound; ++v) {
        total += dfs_count(rem - v, pref + v, v, m);
    }
    return total;
}

/* Count partitions for a single multiplicity m and sum S = n / m */
static seq_t count_for_m(idx_t m, idx_t S)
{
    return dfs_count(S, (idx_t)0, (idx_t)0, m);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1; /* empty partition */
    }

    /* Collect all positive divisors of n.  The number of divisors
       of 10000 is less than 200, so an array of 256 is ample. */
    idx_t divs[256];
    size_t divcnt = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            divs[divcnt++] = d;
            if (d * d != n) {
                divs[divcnt++] = n / d;
            }
        }
    }

    seq_t result = (seq_t)0;
    for (size_t i = 0; i < divcnt; ++i) {
        idx_t m = divs[i];           /* multiplicity */
        idx_t S = n / m;             /* sum of distinct parts */
        result += count_for_m(m, S);
    }

    return result;
}

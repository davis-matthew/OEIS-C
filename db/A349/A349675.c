/*
 * A349675  a(n) is the number of attainable partitions of n.
 * https://oeis.org/A349675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n using parts of size 1 or
 *            i*(i+1) for i>=1. This matches the generating function
 *            (1/(1-x))*Π_{i>=1} 1/(1-x^(i*(i+1))) from the OEIS entry
 * Range:     u8 n <= 43, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *part_dp = NULL;   /* DP array: part_dp[s] = a(s) */
static idx_t  part_size = 0;    /* size allocated in part_dp (number of elements) */

/* Update the DP table so that it contains values up to and including n */
static void ensure_dp(idx_t n)
{
    /* If already computed up to n, nothing to do */
    if (n < part_size) return;

    /* Allocate new array of length n+1, initialized to 0 */
    size_t newlen = (size_t)(n + 1);
    seq_t *newdp = (seq_t *)calloc(newlen, sizeof(seq_t));
    if (!newdp) { /* out of memory */
        exit(1);
    }
    newdp[0] = 1; /* a(0) = 1 */

    /* Process coin value 1 first (unlimited 1s) */
    for (size_t s = 1; s <= (size_t)n; ++s) {
        newdp[s] += newdp[s - 1];
    }

    /* Process coin values i*(i+1) for i >= 1 */
    for (idx_t i = 1;; ++i) {
        idx_t v = i * (i + 1);   /* part value */
        if (v > n) break;
        for (size_t s = (size_t)v; s <= (size_t)n; ++s) {
            newdp[s] += newdp[s - (size_t)v];
        }
    }

    /* Replace old table with the new one */
    if (part_dp) free(part_dp);
    part_dp   = newdp;
    part_size = newlen;
}

/* Public function: returns a(n) */
seq_t a(idx_t n)
{
    ensure_dp(n);
    return part_dp[(size_t)n];
}

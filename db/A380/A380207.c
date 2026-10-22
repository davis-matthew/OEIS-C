/*
 * A380207  Rank of the partition of n formed by the terms of its binary
 *          expansion from largest to smallest.
 * https://oeis.org/A380207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 + Σ_{p=1}^{highest_power(n)-1} P(n-p, p) where P(s, m)
 *            is the number of partitions of s with parts ≤ m. The
 *            implementation builds a triangular table of P(s, m) values up to
 *            the largest n requested and then evaluates the rank directly
 * Range:     u8 n <= 16, u16 n <= 43, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   highest_power
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/highest_power.h"

/* triangular table storage for P(s, m).  For each s, we store values
   for m = 0 … s  (inclusive).  The offset of P(s, m) is
        offset = s*(s+1)/2 + m
   Table is rebuilt whenever a larger n than any previous call is
   required. */
static idx_t dp_Nmax = 0;        /* maximum n for which the table is built */
static seq_t *dp_table = NULL;   /* flat array holding the triangular table */

/* rebuild the table for all s ≤ N and m ≤ s */
static void rebuild_dp(idx_t N)
{
    if (dp_table != NULL) {
        free(dp_table);
        dp_table = NULL;
    }
    /* total number of entries: (N+1)(N+2)/2 */
    size_t total = (size_t)(N + 1) * (N + 2) / 2;
    dp_table = (seq_t *)malloc(total * sizeof(seq_t));
    if (dp_table == NULL) {
        /* allocation failed – terminate */
        abort();
    }

    /* base case: P(0,0) = 1 */
    dp_table[0] = (seq_t)1;

    /* compute rows in increasing order of s */
    for (idx_t s = 1; s <= N; ++s) {
        size_t base = (size_t)s * (s + 1) / 2;  /* start index of row s */
        dp_table[base + 0] = (seq_t)0;          /* P(s,0) = 0 for s>0 */

        for (idx_t m = 1; m <= s; ++m) {
            /* P(s, m-1) is already filled in this row */
            seq_t a = dp_table[base + (m - 1)];
            /* P(s-m, m) (or P(s-m, s-m) if m > s-m) */
            seq_t b;
            if (m <= s - m) {
                size_t base_sm = (size_t)(s - m) * (s - m + 1) / 2;
                b = dp_table[base_sm + m];
            } else {
                size_t base_sm = (size_t)(s - m) * (s - m + 1) / 2;
                b = dp_table[base_sm + (s - m)];
            }
            dp_table[base + m] = a + b;
        }
    }
    dp_Nmax = N;
}

/* get P(s, m) from the triangular table (m ≤ s) */
static inline seq_t get_P(idx_t s, idx_t m)
{
    if (s == 0) return (seq_t)1;   /* P(0, m) = 1 for any m */
    if (m == 0) return (seq_t)0;   /* P(s>0, 0) = 0 */
    if (m > s) m = s;              /* beyond s same as m = s */
    size_t offset = (size_t)s * (s + 1) / 2 + m;
    return dp_table[offset];
}

/* public function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not defined for n=0, but defensive */

    /* ensure DP table covers this n */
    if (dp_Nmax < n)
        rebuild_dp(n);

    seq_t rank = (seq_t)1;         /* rank starts at 1 */
    idx_t remaining = n;

    while (remaining > 0) {
        idx_t part = highest_power(remaining);
        /* count all partitions whose first part is < part */
        for (idx_t p = 1; p < part; ++p) {
            idx_t rem_part = remaining - p;
            /* partitions of rem_part with parts ≤ p */
            rank += get_P(rem_part, p);
        }
        remaining -= part;        /* move to next part of binary partition */
    }
    return rank;
}

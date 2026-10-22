/*
 * A342713  Partition the integers from 1 to n into three groups with
 *          consecutive numbers, then a(n) is the maximum value of the sum of
 *          the numbers in the second group multiplied by the minimum of the
 *          sum of the numbers in the first and third groups.
 * https://oeis.org/A342713
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   triangular_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A342713: partition 1..n into three consecutive blocks, maximizing
   (sum of second block) × min(sum of first block, sum of third block).  The
   algorithm iterates over the end of the first block, determines the
   critical position where the third-block sum drops below the first-block sum
   (via a floor-sqrt computation), and evaluates the product at that
   position and the next one.  This yields an O(n log n) time algorithm
   suitable for n up to at least 10 000. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/triangular_uint.h"

/* --------------------- Integer square root (seq_t) --------------------- */
static inline seq_t floor_sqrt(seq_t x)
{
    if (x <= 1) return x;
    seq_t low  = 0;
    /* 65536 > sqrt(8*max_total+1) for all n <= 10000 */
    seq_t high = ((seq_t)1) << 16;
    while (low + 1 < high) {
        seq_t mid     = low + (high - low) / 2;
        seq_t mid_sq  = mid * mid;
        if (mid_sq <= x)  low = mid;
        else              high = mid;
    }
    return low;
}

/* --------------------- Main sequence function --------------------- */
seq_t a(idx_t n)
{
    /* n ≥ 3 by the sequence definition; no explicit check needed. */
    seq_t total = triangular_uint(n);  /* Sum of 1..n */
    seq_t best  = 0;                   /* Maximum product found */

    for (idx_t i = 1; i <= n - 2; ++i) {
        seq_t Ti    = triangular_uint(i);   /* Sum of first block */
        seq_t limit = total - Ti;           /* Needed for j search */

        /* Find largest j (i+1 ≤ j ≤ n‑1) with T(j) ≤ limit.
           Compute via floor-sqrt of (8*limit+1) as described in OEIS comment. */
        seq_t expr     = limit * 8 + 1;
        seq_t sqrt_val = floor_sqrt(expr);
        seq_t j0_seq   = (sqrt_val - 1) / 2;
        idx_t j0       = (idx_t)j0_seq;

        if (j0 < i + 1) j0 = i + 1;
        if (j0 > n - 1) j0 = n - 1;

        /* Evaluate j = j0 and j = j0+1 (if valid) to capture optimum. */
        for (idx_t jj = j0; jj <= j0 + 1; ++jj) {
            if (jj <= i || jj > n - 1) continue;
            seq_t Tj = triangular_uint(jj);
            seq_t S2 = Tj - Ti;
            seq_t S3 = total - Tj;
            seq_t prod;
            if (S3 >= Ti) prod = S2 * Ti;
            else          prod = S2 * S3;
            if (prod > best) best = prod;
        }
    }
    return best;
}

/*
 * A340979  Number of ways to write n as an ordered sum of 6 positive cubes.
 * https://oeis.org/A340979
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in (∑_{k≥1} x^{k^3})^6 (number of ways
 *            to write n as an ordered sum of 6 positive cubes)
 * Range:     u8 n <= 225, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* When compiled as C++, seq_t may be an arbitrary‑precision integer
   class.  The following code uses only integer arithmetic and
   pointer indexing that works for the native integer types and for
   such a class because all indices are native unsigned integers. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 6; for smaller n the result
       is zeros automatically (no positive cube sum can reach a value
       below 6). */
    if (n < 6) {
        return 0;
    }

    /* Static state: DP table and the largest n for which it is valid.
       DP is stored as a single flat array of (7 * (dp_max+1))
       elements.  dp[ k * (dp_max+1) + s ] = number of ways to write
       s as an ordered sum of k positive cubes.  Only k = 0..6 are needed. */
    static seq_t *dp = NULL;
    static idx_t  dp_max = 0;

    /* If the requested n is larger than what we have already computed,
       rebuild the table up to n.  The rebuild is cheap (≲ 10^6
       operations for n = 10 000). */
    if (dp == NULL || n > dp_max) {
        /* Compute all cubes ≤ n.  For n ≤ 10 000 there are at most 21
           cubes 1³ … 21³ = 9261, which is a very small constant. */
        idx_t cubes[32];
        idx_t cube_count = 0;
        for (idx_t i = 1; ; ++i) {
            idx_t c = (idx_t)i * i * i;
            if (c > n) break;
            cubes[cube_count++] = c;
        }

        /* Allocate new table with 7 rows and (n+1) columns. */
        size_t cols  = (size_t)(n + 1);
        size_t elems = 7 * cols;
        seq_t *newdp = (seq_t*)calloc(elems, sizeof(seq_t));
        if (!newdp) {             /* allocation failure → return 0 silently */
            return 0;
        }

        /* Base case: exactly one way to sum 0 with 0 cubes. */
        newdp[0] = 1;

        /* DP for k = 1 … 6. */
        for (idx_t k = 1; k <= 6; ++k) {
            size_t offset_k   = (size_t)k   * cols;
            size_t offset_km1 = (size_t)(k-1) * cols;
            for (idx_t s = 1; s <= n; ++s) {
                seq_t sum = 0;
                for (idx_t ci = 0; ci < cube_count; ++ci) {
                    idx_t c = cubes[ci];
                    if (c > s) break;
                    sum += newdp[offset_km1 + (size_t)(s - c)];
                }
                newdp[offset_k + (size_t)s] = sum;
            }
        }

        /* Replace the old table (if any) and update the state. */
        free(dp);
        dp     = newdp;
        dp_max = n;
    }

    /* Return the value for k = 6 and total sum = n. */
    size_t cols = (size_t)(dp_max + 1);
    return dp[6 * cols + (size_t)n];
}

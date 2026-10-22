/*
 * A339737  Triangle read by rows where T(n,k) is the number of integer
 *          partitions of n with greatest gap k.
 * https://oeis.org/A339737
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of integer partitions of the integer represented
 *            by the * row of the triangular indexing, whose greatest gap
 *            equals the column index. * The triangle T(r,k) is read
 *            row‑by‑row, with r = 0,1,2,… and k = 0,1,…,r. * This
 *            implementation precomputes the partition numbers p(j,m) for * 0
 *            ≤...
 * Range:     u8 n <= 356, u16 n <= 1325, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* -------------------------------------------------------------------- */
/*  The maximum row to pre‑allocate.  2000 gives > 2 000 000
 *  entries, far more than needed for n up to 10⁶.  Adjust if larger
 *  values of n are required. */
#define MAX_ROW 2000

/* dp[t][s] = number of partitions of s using parts ≤ t */
static seq_t dp[MAX_ROW + 1][MAX_ROW + 1];
static int dp_initialized = 0;

/* -------------------------------------------------------------------- */
/* Return the row number r such that r*(r+1)/2 ≤ idx < (r+1)*(r+2)/2 */
static inline idx_t tri_row(idx_t idx)
{
    /* fast quadratic formula could be used, but a simple loop is
     * sufficient for the expected sizes and keeps type safety. */
    idx_t lo = 0, hi = idx;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri <= idx) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

/* -------------------------------------------------------------------- */
/* Pre‑compute partition numbers dp.  Only executed once. */
static void init_partition_table(void)
{
    if (dp_initialized) return;
    /* dp[0][0] = 1; dp[0][>0] = 0 by static initialization */
    dp[0][0] = (seq_t)1;
    for (idx_t val = 1; val <= MAX_ROW; ++val) dp[0][val] = (seq_t)0;

    for (idx_t part = 1; part <= MAX_ROW; ++part) {
        for (idx_t val = 0; val <= MAX_ROW; ++val) {
            seq_t ways = dp[part - 1][val];
            if (val >= part) {
                ways += dp[part][val - part];
            }
            dp[part][val] = ways;
        }
    }
    dp_initialized = 1;
}

/* -------------------------------------------------------------------- */
/* Count partitions of r with greatest gap exactly k. */
static seq_t count_with_gap(idx_t r, idx_t k)
{
    seq_t result = 0;

    /* Loop over all possible largest part L ≥ k+1 */
    for (idx_t L = k + 1; L <= r; ++L) {
        /* Mandatory sum of parts k+1 … L (each required once) */
        seq_t mandatory = (seq_t)L * (L + 1) / 2
                        - (seq_t)k * (k + 1) / 2;

        if (mandatory > (seq_t)r) break;          /* no further L works */

        /* Remaining value after fixing mandatory parts */
        idx_t rem = r - (idx_t)mandatory;        /* safe: mandatory ≤ r */

        if (rem > MAX_ROW) continue;              /* safety guard */

        /* Partitions of rem using parts ≤ L, excluding part k */
        seq_t ways = dp[L][rem];
        if (k > 0 && rem >= k) {
            ways -= dp[L][rem - k];               /* forbid part k */
        }
        result += ways;
    }
    return result;
}

/* -------------------------------------------------------------------- */
/*  The required function.  n is the linear index in the triangular
 *  numbering of the triangle.  a(0)=1 as per the OEIS entry. */
seq_t a(idx_t n)
{
    init_partition_table();                     /* ensure dp ready */

    /* Convert linear index n to (row, col) */
    idx_t row = tri_row(n);                     /* row number r */
    idx_t offset = row * (row + 1) / 2;
    idx_t col = n - offset;                     /* gap k */

    /* Edge case: the empty partition (row=0, col=0) */
    if (row == 0 && col == 0) {
        return (seq_t)1;
    }

    return count_with_gap(row, col);
}

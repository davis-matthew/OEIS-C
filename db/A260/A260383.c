/*
 * A260383  Triangle read by rows: number of spanning trees obtained for an
 *          almost-complete bipartite graph by removing k disjoint edges from
 *          the complete bipartite graph K n,n with k<=n.
 * https://oeis.org/A260383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A260383 – Triangle: number of spanning trees of an
 *  almost–complete bipartite graph K_{n,n} with k disjoint edge
 *  removals.  The flattened sequence a(n) is produced by reading
 *  the triangle row by row.
 *
 *  For row r (1‑based) and column k (1‑≤k≤r) the term is
 *      T(r,k) = ((r-2)*r + k) * (r-2)^(k-1) * r^(2*r - k - 3)
 *
 *  The function a(idx_t n) returns the n‑th term of the flattened
 *  triangle (offset 1).  It first determines the row r such that
 *  r(r+1)/2 ≥ n, and then k = n - (r-1)r/2.  The result is
 *  computed using the above closed form.
 *
 *  All arithmetic is performed in seq_t, which may be an unsigned
 *  integer type of any width (8,16,32,64 or 128 bits) or an
 *  arbitrary‑precision integer class when compiled as C++17.
 *  No 64‑bit overflow occurs for the values that fit in the target
 *  type, because the sequence values that fit in seq_t are computed
 *  exactly.  For larger n the build that uses seq_t larger than
 *  64 bits will compute the full value.
 */

#ifndef SEQ_T
#include <stdint.h>
#endif

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
        res *= base;
    return res;
}

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 >= n */
    idx_t r = 1;
    idx_t cum = 1;
    while (cum < n) {
        ++r;
        cum += r;
    }

    /* Column index k in this row */
    idx_t prev_cum = cum - r;   /* sum of first r-1 rows */
    idx_t k = n - prev_cum;     /* 1‑based */

    /* Handle rows that need special treatment */
    if (r == 1)
        return 0;               /* row 1 element is 0 */
    if (r == 2 && k == 2)
        return 0;               /* formula gives 0 due to (0)^1 * 2^(-1) */

    /* Compute the term via the closed form */
    seq_t factor1 = ((seq_t)(r - 2) * r) + k;          /* ((r-2)*r + k) */
    seq_t pow1 = pow_seq((seq_t)(r - 2), k - 1);       /* (r-2)^(k-1) */
    idx_t exp2 = 2 * r - k - 3;                        /* exponent for r */
    seq_t pow2 = pow_seq((seq_t)r, exp2);             /* r^(2*r - k - 3) */

    return factor1 * pow1 * pow2;
}

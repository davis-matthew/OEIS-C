/*
 * A261036  Table read by rows: number of complete partitions of n with
 *          largest part = k.
 * https://oeis.org/A261036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, look
 * Formula:   not stated
 * Range:     u8 n <= 187, u16 n <= 940, u32 n <= 6423, u64 n <= 9624,
 *            u128 n <= 9624, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A261036:  Table read by rows – number of complete partitions of n
 *  with largest part = k.  The linear index n is mapped to a table
 *  element (row = r, column = k) by writing the table as
 *
 *          (1,1)
 *          (2,1)
 *          (3,1) (3,2)
 *          (4,1) (4,2)
 *          (5,1) (5,2) (5,3)
 *          (6,1) (6,2) (6,3)
 *          (7,1) (7,2) (7,3) (7,4)
 *          …
 *
 *  and then read it row‑by‑row.  To compute a(n) we
 *  1. convert the linear index to (r,k);
 *  2. ensure that the table D[r][k] (the recurrence table of
 *     complete partitions) is built up to row r;
 *  3. return D[r][k].
 *
 *  The recurrence for D_k(n) (complete partitions of n with largest
 *  part exactly k) is:
 *
 *  D_1(n)  = 1                           (n > 0)
 *  D_k(0)  = 0   for all k
 *  D_k(n)  = 0                              if 1 <= n <= 2k-2
 *  D_k(n)  = D_{k-1}(n-1)           if 2k-1 <= n <= 3k-2
 *  D_k(n)  = D_{k-1}(n-1) + D_k(n-k)   if n >= 3k-1
 *
 *  The table is stored as a ragged array:  D[n][k] for 1<=k<=ceil(n/2).
 *  Memory for the whole table up to the maximum requested row is
 *  allocated once and reused for every call to a().
 */

#include <stdlib.h>
#include <stdint.h>

/*------------------------------------------------------------------
 *  Integer square‑root for unsigned 64‑bit integers.
 *------------------------------------------------------------------*/
static idx_t floor_sqrt_u64(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = 0xFFFFFFFFULL;          /* sqrt(2^64‑1) < 2^32 */
    if (x < (idx_t)0x100000000ULL)     /* x < 2^32, shrink upper bound */
        hi = x;

    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        uint64_t mid_sq = (uint64_t)mid * mid;
        if (mid_sq == x)
            return mid;
        if (mid_sq < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/*------------------------------------------------------------------
 *  Ragged table of complete partitions: D[n][k]  (1‑based indices)
 *------------------------------------------------------------------*/
static seq_t **D = NULL;          /* array of row pointers */
static idx_t max_row = 0;         /* highest computed row (r) */

/* Ensure that rows 1..n are computed and stored in D. */
static void build_table(idx_t n)
{
    if (n <= max_row)
        return;

    /* Allocate / grow row pointer array */
    D = (seq_t **)realloc(D, (n + 1) * sizeof(seq_t *));
    for (idx_t i = max_row + 1; i <= n; ++i) {
        idx_t len = (i + 1) / 2;               /* number of columns in row i */
        D[i] = (seq_t *)malloc((len + 1) * sizeof(seq_t)); /* indices 1..len */
        D[i][1] = (seq_t)1;                   /* D_1(n) = 1 for all n>0 */

        for (idx_t k = 2; k <= len; ++k) {
            if (i < 2 * k - 1) {
                D[i][k] = 0;
            } else if (i <= 3 * k - 2) {
                D[i][k] = D[i - 1][k - 1];
            } else {
                D[i][k] = D[i - 1][k - 1] + D[i - k][k];
            }
        }
    }
    max_row = n;
}

/*------------------------------------------------------------------
 *  Main routine:  a(idx) returns the requested OEIS term.
 *------------------------------------------------------------------*/
seq_t a(idx_t idx)
{
    /* 1. Map linear index to table coordinates (row r, column k). */
    idx_t t = floor_sqrt_u64(idx);            /* floor(sqrt(idx)) */
    idx_t t2 = t * t;
    idx_t r;                                  /* row index */
    if (idx <= t2) {
        r = 2 * t - 1;
    } else if (idx <= t * (t + 1)) {
        r = 2 * t;
    } else {
        r = 2 * (t + 1) - 1;
    }

    /* Cumulative count up to row r */
    idx_t len_r = (r + 1) / 2;                /* number of elements in row r */
    idx_t C_r;
    if ((r & 1) == 0) {                      /* r even: C = t*(t+1) */
        idx_t t_r = r / 2;
        C_r = t_r * (t_r + 1);
    } else {                                 /* r odd:  C = t*t where t=(r+1)/2 */
        idx_t t_r = (r + 1) / 2;
        C_r = t_r * t_r;
    }
    idx_t prevC = C_r - len_r;                /* cumulative count before row r */
    idx_t k = idx - prevC;                    /* column (1‑based) */

    /* 2. Ensure the table is built up to row r. */
    build_table(r);

    /* 3. Return the required table entry. */
    return D[r][k];
}

/*
 * A362791  Triangle of numbers read by rows, T(n, k) =
 *          (n*(n-1)*(n-2))*Stirling2(k, 3), for n >= 1 and 1 <= k <= n.
 * https://oeis.org/A362791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (r*(r-1)*(r-2))*Stirling2(k,3) where r and k are defined
 *            by r*(r-1)/2 < n ≤ r*(r+1)/2 and k = n - r*(r-1)/2. T(r,k) =
 *            (r*(r-1)*(r-2))*Stirling2(k,3)
 * Range:     u8 n <= 13, u16 n <= 34, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static tables for powers of two and Stirling numbers S(k,3) */
static seq_t *pow2_tbl   = NULL;
static idx_t  pow2_len   = 0;
static seq_t *stir3_tbl  = NULL;
static idx_t  stir3_len  = 0;

/* ensure the tables are computed up to index 'n' inclusively */
static void ensure_tables(idx_t n)
{
    if (stir3_len > n) return;          /* already sufficient */

    /* allocate new size */
    idx_t new_len = n + 1;               /* indices 0..n */
    if (pow2_len == 0) {
        /* initial allocation */
        pow2_tbl  = (seq_t*)malloc(new_len * sizeof(seq_t));
        stir3_tbl = (seq_t*)malloc(new_len * sizeof(seq_t));
        pow2_len  = new_len;
        stir3_len = new_len;
        pow2_tbl[0] = (seq_t)1;
        for (idx_t i = 1; i < new_len; ++i)
            pow2_tbl[i] = pow2_tbl[i-1] * ((seq_t)2);

        stir3_tbl[0] = 0;
        stir3_tbl[1] = 0;
        stir3_tbl[2] = 0;
        for (idx_t i = 3; i < new_len; ++i)
            stir3_tbl[i] =
                3 * stir3_tbl[i-1] + (pow2_tbl[i-2] - ((seq_t)1));
    }
    else {
        /* expand existing tables */
        pow2_tbl  = (seq_t*)realloc(pow2_tbl,  new_len * sizeof(seq_t));
        stir3_tbl = (seq_t*)realloc(stir3_tbl, new_len * sizeof(seq_t));
        /* new pow2 entries */
        for (idx_t i = pow2_len; i < new_len; ++i)
            pow2_tbl[i] = pow2_tbl[i-1] * ((seq_t)2);
        /* new Stirling entries */
        for (idx_t i = stir3_len; i < new_len; ++i)
            stir3_tbl[i] =
                3 * stir3_tbl[i-1] + (pow2_tbl[i-2] - ((seq_t)1));
        pow2_len  = new_len;
        stir3_len = new_len;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* below offset not used */

    /* find the row r (1‑based) such that r*(r+1)/2 ≥ n */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;

    /* cumulative elements before this row */
    idx_t prev = r * (r - 1) / 2;
    idx_t k    = n - prev;           /* 1‑based position within the row */

    /* if fewer than 3 boxes or 3 items, the entry is zero */
    if (r < 3 || k < 3) return (seq_t)0;

    /* current Stirling number S(k,3) */
    ensure_tables(k);
    seq_t s = stir3_tbl[k];

    /* factor n*(n-1)*(n-2) */
    seq_t factor = (seq_t)r * (r - 1) * (r - 2);

    return factor * s;
}

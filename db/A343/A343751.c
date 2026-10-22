/*
 * A343751  A(n,k) is the sum of all compositions [c_1, c_2, ..., c_k] of n
 *          into k nonnegative parts encoded as Product_{i=1..k}
 *          prime(i)^(c_i); square array A(n,k), n>=0, k>=0, read by
 *          antidiagonals.
 * https://oeis.org/A343751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the square array A(n,k) by
 *            antidiagonals. A(n,k) = Σ_{c₁+…+c_k=n} ∏_{i=1..k} prime(i)^{c_i}
 * Range:     u8 n <= 23, u16 n <= 48, u32 n <= 111, u64 n <= 264,
 *            u128 n <= 651, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/get_prime.h"

/*  Dynamically sized table that stores A(j,i) for 0 ≤ j ≤ max_j, 0 ≤ i ≤ max_i.
    dp[j * cols + i] gives the value A(j,i).  */
static seq_t *dp    = NULL;
static size_t dp_rows = 0;          /* number of rows   (max_j + 1)   */
static size_t dp_cols = 0;          /* number of columns (max_i + 1) */

/*  Ensure that the table contains all entries up to (j_needed,i_needed).
    If the current allocation is too small, a new table is allocated and
    all required values are computed.  */
static void ensure_dp(idx_t j_needed, idx_t i_needed)
{
    size_t jreq = (size_t)j_needed + 1;
    size_t ireq = (size_t)i_needed + 1;

    if (dp != NULL && jreq <= dp_rows && ireq <= dp_cols)
        return;                           /* already big enough */

    /*  Allocate a new table; old table will be freed automatically.  */
    dp_rows = jreq;
    dp_cols = ireq;
    free(dp);
    dp = (seq_t *)calloc(dp_rows * dp_cols, sizeof(seq_t));
    if (dp == NULL)                         /* memory exhausted – abort */
        return;

    /*  Row 0: A(0,0)=1, A(0,i>0)=0  */
    dp[0] = (seq_t)1;
    for (size_t ic = 1; ic < dp_cols; ++ic)
        dp[ic] = (seq_t)0;

    /*  Compute rows j = 1 … jreq-1  */
    for (size_t j = 1; j < dp_rows; ++j)
    {
        /*  prime_j = prime(j), j is 1‑indexed.  */
        uint64_t prime = get_prime((idx_t)j);
        dp[j * dp_cols + 0] = (seq_t)1;          /* A(j,0)=1  */

        for (size_t i = 1; i < dp_cols; ++i)
        {
            seq_t sum   = (seq_t)0;
            seq_t power = (seq_t)1;              /* prime^0  */
            for (size_t t = 0; t <= i; ++t)
            {
                /*  A(j,i) = Σ_{t=0..i} A(j-1,i-t) * prime^t  */
                sum += dp[(j-1) * dp_cols + (i - t)] * power;
                power *= prime;
            }
            dp[j * dp_cols + i] = sum;
        }
    }
}

/*  Return the n‑th term of OEIS sequence A343751.  */
seq_t a(idx_t n)
{
    /*  Find the diagonal t that contains index n.
        The cumulative number of elements up to diagonal t-1 is t(t+1)/2.  */
    idx_t t = 0;
    while ( ((t + 1) * (t + 2)) / 2 <= n )
        ++t;

    idx_t prev = (t * (t + 1)) / 2;      /* elements before diagonal t  */
    idx_t d    = n - prev;                /* offset inside the diagonal  */

    idx_t i = d;        /* row index   */
    idx_t j = t - d;    /* column index */

    /*  Ensure that A(j,i) has been computed.  */
    ensure_dp(j, i);

    return dp[j * dp_cols + i];
}

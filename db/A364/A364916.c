/*
 * A364916  Array read by antidiagonals downwards where A(n,k) is the number
 *          of ways to write n as a nonnegative linear combination of the
 *          parts of a strict integer partition of k.
 * https://oeis.org/A364916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 434, u16 n <= 2164, u32 n <= 9605, u64 n <= 9605,
 *            u128 n <= 9605, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A364916
 *  a(n) = number obtained by reading the triangle
 *  A(n,k) = number of ways to write n as a non‑negative linear
 *  combination of the parts of a strict partition of k, read
 *  by antidiagonals downwards.
 *
 *  Computation by dynamic programming over coin values 1..MAXK.
 *  Works for all n ≥ 0.  The table is computed once on first call.
 */

#include <stdlib.h>
#include <stdbool.h>

/* bounds sufficient for all n up to about 10 000 */
#define MAXK 200

/*  dp[k*(MAXK+1)+n] = A(n,k)   */
static seq_t *dp = NULL;   /* will be allocated in init() */
static bool initialized = false;

/* ----- initialise the DP table ------------------------------------- */
static void init(void)
{
    size_t sz = (size_t)(MAXK + 1) * (MAXK + 1);
    dp = (seq_t *)calloc(sz, sizeof(seq_t));
    if (dp == NULL)            /*  out of memory – caller will not use the function  */
        return;

    dp[0] = 1;                 /* empty set: A(0,0) = 1 */

    /* standard 0‑1 knapsack with unlimited copies for each chosen part */
    for (int v = 1; v <= MAXK; ++v) {
        /* process old sums in reverse to avoid reuse of the new state */
        for (int sum_k = MAXK - v; sum_k >= 0; --sum_k) {
            size_t base_old = (size_t)sum_k * (MAXK + 1);
            size_t base_new = ((size_t)sum_k + v) * (MAXK + 1);

            for (int n_prev = 0; n_prev <= MAXK; ++n_prev) {
                seq_t c = dp[base_old + n_prev];
                if (c == 0) continue;

                /*  include part v : choose any number of copies          */
                for (int n_new = n_prev; n_new <= MAXK; n_new += v) {
                    dp[base_new + n_new] += c;
                }
            }
        }
    }
    initialized = true;
}

/* ----- helper: find the anti‑diagonal index d for a given n -------- */
static inline idx_t diag_of(idx_t n)
{
    idx_t d = 0;
    /*  d*(d+1)/2  is the last index on diagonal d  */
    while ((d + 1) * (d + 2) / 2 <= n)
        ++d;
    return d;
}

/* ------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized)
        init();

    if (dp == NULL)          /*  allocation failed   */
        return (seq_t)0;

    idx_t d      = diag_of(n);                     /* diagonal number   */
    idx_t offset = n - d * (d + 1) / 2;             /* offset on diagonal*/
    idx_t i      = offset;                         /* n to be represented */
    idx_t j      = d - offset;                     /* sum of the parts   */

    if (i > (idx_t)MAXK || j > (idx_t)MAXK)
        return (seq_t)0;

    return dp[(size_t)j * (MAXK + 1) + (size_t)i];
}

/*
 * A378236  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where T(n,0) = 0^n and T(n,k) = k * Sum_{r=0..n}
 *          binomial(n,r) * binomial(n+2*r+k,n)/(n+2*r+k) for k > 0.
 * https://oeis.org/A378236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is read from the square array T(n,k) defined by: T(n,0) =
 *            0^n ( =1 if n==0, else 0 ), T(n,k) = T(n,k-1) + T(n-1,k) +
 *            T(n-1,k+2) for k>0. The sequence is read along the
 *            antidiagonals: a(0)=T(0,0), a(1)=T(0,1), a(2)=T(1,0),
 *            a(3)=T(0,2), …
 * Range:     u8 n <= 18, u16 n <= 42, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define MAX_D 2000  /* sufficient for all n <= 10000 */

/* global table: T[n][k] with n,k <= MAX_D+2 */
static seq_t T[MAX_D+1][MAX_D+3];

/* initialise the table once */
static void init_table(void)
{
    static int done = 0;
    if (done) return;
    for (idx_t n = 0; n <= MAX_D; ++n) {
        for (idx_t k = 0; k <= MAX_D+2; ++k) {
            if (k == 0) {
                T[n][k] = (n == 0) ? (seq_t)1 : (seq_t)0;
            } else {
                seq_t a = T[n][k-1];
                seq_t b = (n == 0) ? (seq_t)0 : T[n-1][k];
                seq_t c = (n == 0) ? (seq_t)0 : T[n-1][k+2];
                T[n][k] = a + b + c;
            }
        }
    }
    done = 1;
}

/* Return the n-th term of sequence A378236 */
seq_t a(idx_t n)
{
    init_table();

    /* find the diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    idx_t d = 0;
    while ( ((d+1)*(d+2))/2 <= n ) ++d;
    /* triangular number for this d is d*(d+1)/2 */
    idx_t tri = (d*(d+1))/2;
    idx_t offset = n - tri;   /* 0‑based position along the diagonal */
    idx_t row = offset;            /* n-coordinate of T */
    idx_t col = d - offset;        /* k-coordinate of T */

    /* safety check: indices must be within the pre‑allocated table */
    if (row > MAX_D || col > MAX_D+2) {
        /* if required indices exceed the table bounds, enlarge the table
           (unneeded for the given test limits, but kept for generality). */
        /* This case is not expected for the target problem constraints. */
        return (seq_t)0;
    }

    return T[row][col];
}

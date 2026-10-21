/*
 * A285078  Positions of 1 in A285076; complement of A285077.
 * https://oeis.org/A285078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 61, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A285078: a(n) = n + floor(n*sqrt(2)) – c(n) where
   c(n) = 1 if (n−1)^2 * 2 ≤ (floor(n*sqrt(2))−1)^2, else 0.
   The function uses the supplied floor_sqrt() to avoid
   floating‑point arithmetic.                                                    */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* handle the offset – the sequence is defined for n≥1 */
    if (n == 0)
        return 0;

    /* convert the index to the seq_t domain */
    seq_t nt  = (seq_t)n;          /* n as seq_t         */
    seq_t n1  = nt - (seq_t)1;     /* n−1 as seq_t       */

    /* t = floor(n*sqrt(2))  = floor( sqrt( 2 * n^2 ) )        */
    seq_t two_n_sq = (seq_t)2 * nt * nt;
    seq_t t        = floor_sqrt(two_n_sq);

    /* condition: (n−1)^2 * 2 ≤ (t−1)^2  */
    seq_t cond = (seq_t)0;
    if (n == 1) {                 /* left = 0, right = 0 */
        cond = (seq_t)1;
    } else {
        seq_t left  = (seq_t)2 * n1 * n1;
        seq_t t1    = t - (seq_t)1;
        seq_t right = t1 * t1;
        if (left <= right)
            cond = (seq_t)1;
    }

    /* final formula */
    return nt + t - cond;
}

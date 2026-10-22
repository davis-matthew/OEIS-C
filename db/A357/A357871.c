/*
 * A357871  Total number of n-multisets of semi-Dyck paths from (0,0) to
 *          (n,n-2*j) for j=0..floor(n/2).
 * https://oeis.org/A357871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..floor(n/2)} binomial( B(n,j) + n - 1 , n )
 *            where B(n,j) = ((n - 2*j + 1) / (n - j + 1)) * binomial(n, j)
 *            (ballot number)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation uses the helper function binom from utils (see list in the
    problem statement).  All arithmetic is performed with the seq_t type, which
    supports the usual +, -, *, / operators for arbitrary‑precision values.
    The routine works for any idx_t n; it does not assume monotonic calls.  */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/binom.h"   /* returns seq_t binom(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    seq_t sum = 0;

    /* upper limit for j : floor(n/2) */
    idx_t maxj = n / 2;

    for (idx_t j = 0; j <= maxj; ++j) {
        /* binomial(n, j) as seq_t */
        seq_t bin_n_j = binom(n, j);

        /* compute B(n,j) = ((n-2*j+1)/(n-j+1)) * binom(n, j) */
        idx_t numer_mul = n - 2*j + 1;          /* small integer */
        idx_t denom = n - j + 1;                /* small integer */

        seq_t num = bin_n_j * numer_mul;          /* seq_t * small int */
        seq_t B   = num / denom;                 /* exact division */

        /* compute C(B + n - 1, n) via multiplicative formula:
           C(B + n - 1, n) = product_{i=1..n} (B + i - 1) / i
        */
        seq_t comb = 1;
        for (idx_t i = 1; i <= n; ++i) {
            seq_t term = B + i - 1;             /* seq_t + small int - 1 */
            comb = (comb * term) / i;           /* exact division */
        }

        sum = sum + comb;
    }

    return sum;
}

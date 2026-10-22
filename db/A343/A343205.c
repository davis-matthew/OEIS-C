/*
 * A343205  a(n) is the unique positive integer m such that n <
 *          m*sqrt(A261865(n)) < n + 1.
 * https://oeis.org/A343205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 682, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithmic construction of OEIS sequence A343205.
   For each n>=1 find the smallest integer k>=2 such that there is
   an integer m with n < m*sqrt(k) < n+1.
   For that k the unique m is a(n).  This routine tests successive k
   using only integer arithmetic: for each k compute
       m = floor_sqrt((n+1)^2 / k)
   and accept the first k for which
       n^2 < m^2*k < (n+1)^2            (strict inequalities).
   The value returned is the corresponding m.                                      */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert the index to the derived sequence type. */
    seq_t n_s   = (seq_t)n;              /* n */
    seq_t np1   = n_s + 1;               /* n+1 */
    seq_t n_sq  = n_s * n_s;             /* n^2          */
    seq_t np1_sq = np1 * np1;            /* (n+1)^2      */

    /* Search for the minimal admissible k. */
    for (idx_t k = 2; ; ++k) {
        /* quotient = (n+1)^2 / k  (truncated). */
        seq_t quotient = np1_sq / (seq_t)k;
        if (quotient == 0)                /* k too large: never succeeds */
            continue;

        /* m = floor_sqrt( quotient ) . */
        seq_t m = floor_sqrt(quotient);
        if (m == 0)
            continue;                     /* no positive integer m possible */

        /* Compute m^2 * k and test the strict inequalities. */
        seq_t mk = m * m * (seq_t)k;
        if (mk > n_sq && mk < np1_sq)
            return m;                     /* found the required m */
    }
}

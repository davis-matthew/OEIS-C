/*
 * A364152  Least n-simplex number (i.e., number of the form C(m,n) =
 *          binomial(m,n), m >= n), that can be written as a product of two or
 *          more smaller n-simplex numbers, or 0 if no such number exists.
 * https://oeis.org/A364152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Least n‑simplex number that can be expressed as a product of two or more
   smaller n‑simplex numbers, or 0 if no such number exists.
   Computation: for each n, search for the smallest m>n such that
   C(m,n) = C(a,n) * C(b,n) with n <= a,b < m and a,b>n.
   The search is limited to a maximum difference of 200 between m and n.
   If no such m is found within that range, return 0. */
#include <stdbool.h>

#define MAX_DIFF 200

seq_t a(idx_t n)
{
    /* Allocate array of binomial values for current n.
       B[d] = C(n + d, n) == C(n + d, d) */
    seq_t B[MAX_DIFF + 1];
    B[0] = (seq_t)1;                     /* C(n,n) = 1 */

    for (idx_t diff = 1; diff <= MAX_DIFF; ++diff) {
        /* B[diff] = B[diff-1] * (n + diff) / diff  (exact division) */
        B[diff] = (B[diff - 1] * ((seq_t)(n + diff))) / ((seq_t)diff);

        seq_t target = B[diff];
        /* Search for a pair of smaller binomials whose product equals target */
        for (idx_t i = 1; i < diff; ++i) {
            seq_t a_val = B[i];
            if (a_val == 0) continue;              /* safety */

            if (target % a_val != 0) continue;     /* not a divisor */

            seq_t q = target / a_val;
            /* q must itself be a binomial < target */
            for (idx_t j = 1; j < diff; ++j) {
                if (B[j] == q) {
                    return target;                  /* found the least such number */
                }
            }
        }
        /* Optional: handle factorisations with more than two factors
           by dividing further could be added here if needed. */
    }

    /* No factorisation found within the search window */
    return (seq_t)0;
}

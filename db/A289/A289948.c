/*
 * A289948  a(n) = Sum_{k=0..n} k!^3.
 * https://oeis.org/A289948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} (k!)^3
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* The function returns the n-th term of OEIS sequence A289948.
   It computes the sum of the cubes of factorials up to n in O(n) time,
   using only arithmetic on the user-provided seq_t type.   */
seq_t a(idx_t n)
{
    /* Initialise for k = 0  (0! = 1, (0!)^3 = 1) */
    seq_t fact = (seq_t)1;      /* k!     */
    seq_t cube = (seq_t)1;      /* (k!)^3 */
    seq_t sum  = (seq_t)0;      /* cumulative sum */

    for (idx_t k = 0; k <= n; ++k) {
        /* Add current cube to the sum */
        sum += cube;

        /* If this is the last required term, break */
        if (k == n) {
            break;
        }

        /* Prepare for next k: (k+1)! = k! * (k+1)   */
        seq_t inc = (seq_t)(k + 1);
        fact *= inc;            /* fact now equals (k+1)! */

        /* Update cube: ( (k+1)! )^3 = (k!)^3 * (k+1)^3   */
        seq_t inc_sq = inc * inc;          /* (k+1)^2   */
        seq_t inc_cu = inc_sq * inc;       /* (k+1)^3   */
        cube *= inc_cu;                   /* new cube  */
    }

    return sum;
}

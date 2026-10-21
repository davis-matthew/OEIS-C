/*
 * A250778  Number of (n+1) X (3+1) 0..1 arrays with nondecreasing
 *          x(i,j)+x(i,j-1) in the i direction and nondecreasing absolute
 *          value of x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n+5) - 4*n - 14
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the closed‑form derived from the generating function |
   a(n) = 2^(n+5) - 4*n - 14, which satisfies the recurrence
   a(n) = 4*a(n-1) - 5*a(n-2) + 2*a(n-3) (n ≥ 4). */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* offset 1: a(1) corresponds to n = 1 */
    if (n == 0) return 0;          /* value for n < 1 is irrelevant */
    seq_t power = pow2(n + 5);     /* 2^(n+5) */
    seq_t sub = ((seq_t)4 * n) + 14;
    return power - sub;
}

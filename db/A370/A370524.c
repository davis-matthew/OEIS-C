/*
 * A370524  Number of permutations of [n] having exactly one adjacent 2-cycle.
 * https://oeis.org/A370524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)-1} (-1)^k * (n-k-1)! / k!
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   factorial_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factorial_n.h"

seq_t a(idx_t n) {
    if (n <= 1) return 0;          /* a(0)=0, a(1)=0 */

    /* term_0 = (n-1)!  */
    seq_t term = factorial_n(n - 1);
    seq_t sum  = term;             /* add the first (positive) term */
    int sign = 1;                  /* sign for current term; starts +1 */

    /* We need terms k = 1 .. floor(n/2)-1 */
    idx_t kmax = n / 2 - 1;         /* n >= 2 ensures n/2 >= 1 */
    for (idx_t k = 0; k < kmax; ++k) {
        /* Compute next term magnitude: term_{k+1} = term_k / ((n-k-1)*(k+1)) */
        seq_t denom = (seq_t)(n - k - 1) * (k + 1);
        term = term / denom;

        /* Flip sign for the next term */
        sign = -sign;
        if (sign > 0)
            sum += term;
        else
            sum -= term;
    }
    return sum;
}

/*
 * A343009  a(n) = (n^(2n)-1)/(n^2-1) for n > 1, a(1) = 1.
 * https://oeis.org/A343009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} n^(2*k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t n_sq  = n_seq * n_seq;   /* n^2 */

    seq_t sum  = 0;
    seq_t term = 1;                /* term = n^(2*i) */

    for (idx_t i = 0; i < n; ++i) {
        sum  += term;              /* add current term */
        term *= n_sq;              /* prepare next term n^(2*(i+1)) */
    }

    return sum;
}

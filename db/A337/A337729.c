/*
 * A337729  a(n) = (4*n+2)! * Sum_{k=0..n} 1 / (4*k+2)!.
 * https://oeis.org/A337729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4*n+2)! * Sum_{k=0..n} 1 / (4*k+2)!
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum  = 0;
    seq_t term = 1;

    for (idx_t k = n;; --k) {
        sum += term;
        if (k == 0) break;          /* finished with k = 0 term */

        idx_t m = k - 1;            /* next term index (k-1)                 */
        seq_t p  = (seq_t)(4 * m + 3);
        p *= (seq_t)(4 * m + 4);
        p *= (seq_t)(4 * m + 5);
        p *= (seq_t)(4 * m + 6);

        term *= p;                  /* term_{k-1} = term_k * product           */
    }

    return sum;
}

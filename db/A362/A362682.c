/*
 * A362682  Expansion of e.g.f. exp(-LambertW(-x))/(1+x).
 * https://oeis.org/A362682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 0 a(n) = (n + 1)^(n - 1) - n * a(n - 1) for n ≥
 *            2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    seq_t prev = (seq_t)0;  /* a(1) */
    seq_t curr = 0;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t pow_val  = pow_seq((seq_t)(i + 1), (int)(i - 1));
        seq_t prod     = (seq_t)i * prev;          /* n * a(n-1) */
        curr          = pow_val - prod;            /* (n+1)^(n-1) - n*a(n-1) */
        prev          = curr;                      /* store a(i) */
    }
    return curr;
}

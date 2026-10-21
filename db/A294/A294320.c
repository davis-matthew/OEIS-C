/*
 * A294320  a(n) = Product_{k=0..n} (4*k + 1)!.
 * https://oeis.org/A294320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{k=0..n} (4*k + 1)!
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t prod = (seq_t)1;   /* product of factorials so far         */
    seq_t fact = (seq_t)1;   /* current factorial value              */
    idx_t  m   = (idx_t)1;   /* next integer to multiply into fact   */

    for (idx_t k = (idx_t)0; k <= n; ++k) {
        idx_t target = 4 * k + (idx_t)1;  /* 4*k+1 number whose factorial we need */

        /* Build factorial(4*k+1) incrementally                */
        while (m <= target) {
            fact = seq_mul(fact, (seq_t)m);
            ++m;
        }

        /* Multiply into overall product                       */
        prod = seq_mul(prod, fact);
    }

    return prod;
}

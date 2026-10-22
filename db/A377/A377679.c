/*
 * A377679  Number of subwords of the form DDD in nondecreasing Dyck paths of
 *          length 2n.
 * https://oeis.org/A377679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*F(2*n-3) - L(2*n-2) + 2^(n-2)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   fib, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n < 3) return 0;

    idx_t idx_f1 = 2*n - 3;    /* F(2*n-3) */
    idx_t idx_f2 = 2*n - 1;    /* F(2*n-1) */

    seq_t f1 = fib(idx_f1);    /* F(2*n-3) */
    seq_t f2 = fib(idx_f2);    /* F(2*n-1) */

    seq_t term1 = (seq_t)n * f1;      /* n*F(2*n-3) */
    seq_t L     = f1 + f2;            /* L(2*n-2) = F(2*n-3)+F(2*n-1) */
    seq_t term3 = pow2(n - 2);        /* 2^(n-2) */

    return term1 - L + term3;
}

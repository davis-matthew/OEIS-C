/*
 * A290140  The number of maximal subsemigroups of the Jones monoid on the set
 *          [1..n].
 * https://oeis.org/A290140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * Fib(n-1) + 2n - 3, n > 2 with base values a(1)=1,
 *            a(2)=2 (Fib(0)=0, Fib(1)=1)
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   fib_n_minus_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib_n_minus_one.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    seq_t fib = fib_n_minus_one(n);
    seq_t term = (seq_t)(2 * n - 3) + (fib << 1);  /* 2*fib */
    return term;
}

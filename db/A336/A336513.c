/*
 * A336513  a(n) = Sum_{i=1..n} Product_{j=(i-1)*n+1..i*n} j.
 * https://oeis.org/A336513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} Product_{j=(i-1)*n+1..i*n} j
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A336513. */
#include "../utils/mul_small.h"

seq_t a(idx_t n) {
    seq_t sum = 0;          /* result accumulator */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t term = 1;            /* product of the current block */
        idx_t start = (i - 1) * n + 1;
        idx_t end   = i * n;
        for (idx_t j = start; j <= end; ++j) {
            term = mul_small(term, (unsigned int)j);
        }
        sum += term;          /* add block product to total sum */
    }
    return sum;
}

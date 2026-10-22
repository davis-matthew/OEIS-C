/*
 * A305436  Number of divisors of n of the form 2^k + 1 for k >= 0.
 * https://oeis.org/A305436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of n of the form 2^k + 1 (k >= 0)
 * Range:     u8 n <= 105, u16 n <= 9650, u32 n <= 9650, u64 n <= 9650,
 *            u128 n <= 9650, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: iterate over all Fermat numbers up to n and test divisibility. */

#include <stdint.h>
#include "../utils/pow2.h"

/* Count divisors of n that are of the form 2^k+1 */
seq_t a(idx_t n)
{
    if (n < 2) {
        /* a(1) = 0, and for n = 0 (out of range) return 0 */
        return 0;
    }
    seq_t count = 0;
    idx_t nn = (idx_t)n;  /* n fits into idx_t */
    for (uint64_t k = 0;; ++k) {
        /* compute 2^k + 1 */
        seq_t f = pow2(k) + 1;
        if (f > nn)            /* f can't be a divisor if it exceeds n */
            break;
        if (nn % (idx_t)f == 0)
            count++;
    }
    return count;
}

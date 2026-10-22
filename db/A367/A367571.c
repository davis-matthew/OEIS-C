/*
 * A367571  a(n) = Product_{k=0..n} (7*k)! / k!^7.
 * https://oeis.org/A367571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (7*k)! / k!^7
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*    = Product_{k=0..n} \prod_{j=2..7} binomial(j*k, k) */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;
    for (idx_t k = 0; k <= n; ++k) {
        for (idx_t j = 2; j <= 7; ++j) {
            res = seq_mul(res, binomial(j * k, k));
        }
    }
    return res;
}

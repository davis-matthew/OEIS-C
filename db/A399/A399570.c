/*
 * A399570  a(n) = Product_{k=1..n} (2*k-1)^k.
 * https://oeis.org/A399570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} (2*k-1)^k
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n < 1)   /* offset starts at 1; below this is undefined */
        return 0;

    seq_t result = 1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t base = (seq_t)(2*k - 1);   /* odd number (2k-1) */
        seq_t pow_val = seq_pow(base, k);/* (2k-1)^k */
        result = seq_mul(result, pow_val);
    }
    return result;
}

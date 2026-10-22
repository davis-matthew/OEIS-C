/*
 * A345160  a(n) = Product_{k=1..n} sigma_3(k).
 * https://oeis.org/A345160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} sigma_3(k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   seq_mul, sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/sigma_k.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    seq_t prod = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k)
        prod = seq_mul(prod, sigma_k(k, 3));
    return prod;
}

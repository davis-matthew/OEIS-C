/*
 * A277456  a(n) = 1 + Sum_{k=1..n} binomial(n,k) * 3^k * k^k.
 * https://oeis.org/A277456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=1..n} binomial(n,k) * 3^k * k^k
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   binomial, pow3, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required helpers */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */
#include "../utils/pow3.h"       /* seq_t pow3(idx_t exp); */
#include "../utils/pow_uint.h"   /* seq_t pow_uint(uint64_t base, idx_t exp); */

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    seq_t sum = (seq_t)1;          /* start with the leading +1 */

    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t bin = binomial(n, k);          /* C(n,k) */
        seq_t p3  = pow3(k);                 /* 3^k */
        seq_t pk  = pow_uint((uint64_t)k, k);/* k^k */

        seq_t term = bin * p3;
        term = term * pk;
        sum = sum + term;
    }
    return sum;
}

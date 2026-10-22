/*
 * A344902  Number of open tours by a biased rook on a specific f(n) X 1
 *          board, where f(n) = A070941(n) and cells are colored white or
 *          black according to the binary representation of 2n.
 * https://oeis.org/A344902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A000120(n)! * (1 + A000120(n))^(A023416(n) + 1)
 * Range:     u8 n <= 18, u16 n <= 134, u32 n <= 2558, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          A000120(n) = popcount(n)                                */
/*          A023416(n) = bitlen(n) - popcount(n)                    */
/*          a(0) = 1                                                */

#include <stdint.h>
#include "../utils/big_pow.h"

static inline idx_t bitlen_u64(idx_t n)
{
    if (n == 0) return 0;
    /* number of bits needed to represent n */
    return 64 - __builtin_clzll(n);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    const idx_t pop  = __builtin_popcountll(n);
    const idx_t bits = bitlen_u64(n);
    const idx_t zeros = bits - pop;

    /* factorial of pop (A000120(n)!) */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= pop; ++i)
        fact *= (seq_t)i;

    /* (1 + pop)^(zeros + 1)  */
    const seq_t base = (seq_t)(pop + 1);
    const seq_t exp  = (seq_t)(zeros + 1);
    const seq_t power = big_pow(base, exp);

    return fact * power;
}

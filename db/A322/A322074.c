/*
 * A322074  Maximum number of codewords a binary self-dual code of length 4n
 *          can have with Hamming weight 2n (half of length).
 * https://oeis.org/A322074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2^(2n-1) + (n even ? binomial(2n,n) : 0)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow_uint.h"     /* seq_t pow_uint(uint64_t base, idx_t exp); */
#include "../utils/binomial.h"    /* seq_t binomial(idx_t a, idx_t b); */

seq_t a(idx_t n)
{
    /* Guard against a zero argument (outside the offset) */
    if (n == 0) return 0;

    /* 2^(2n-1) */
    seq_t pow2 = pow_uint(2ull, 2*n - 1);

    /* If n is even add the central binomial coefficient */
    if ((n & 1) == 0) {
        seq_t central = binomial(2*n, n);
        return pow2 + central;
    }
    return pow2;
}

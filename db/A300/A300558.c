/*
 * A300558  a(n) is the concatenation of n written in base 2 n times, n-1
 *          written in base 2 n-1 times, ..., 1 written in base 2 once.
 * https://oeis.org/A300558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = decimal integer obtained by concatenating, * for k = n
 *            down to 1, the binary representation of k repeated k times. *
 *            The concatenated decimal digits are interpreted as a decimal
 *            integer
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    seq_t result = 0;
    const seq_t TEN = 10;

    for (idx_t k = n; k >= 1; --k) {
        /* bit length of k */
        idx_t L = 0;
        idx_t t = k;
        while (t) { ++L; t >>= 1; }

        /* 10^L and 10^(L*k) */
        seq_t ten_pow_L   = pow_seq(TEN, L);
        seq_t ten_pow_Lk  = pow_seq(ten_pow_L, k);

        /* geometric series factor: (10^(L*k)-1)/(10^L-1) */
        seq_t numer = ten_pow_Lk - 1;
        seq_t denom = ten_pow_L - 1;
        seq_t block_multiplier = numer / denom;

        /* decimal value of the binary representation of k */
        seq_t p = 0;
        for (idx_t i = L; i > 0; --i) {
            seq_t bit = (k >> (i - 1)) & 1;
            p = p * TEN + bit;
        }

        /* block decimal value */
        seq_t block_dec = p * block_multiplier;

        /* append block to the current result */
        result = result * ten_pow_Lk + block_dec;
    }

    return result;
}

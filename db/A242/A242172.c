/*
 * A242172  a(n) = 2^n*binomial((n + 1 + (n mod 2))/2, 1/2).
 * https://oeis.org/A242172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+2)*binomial(n+1, ceil(n/2))/2
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 28, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the provided binomial function from utils/binomial.h */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* ceil(n/2) == (n+1)/2 with integer division */
    idx_t k = (n + 1) / 2;
    seq_t bin = binomial(n + 1, k);
    seq_t res = bin * ((seq_t)n + 2) / 2;
    return res;
}

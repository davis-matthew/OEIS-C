/*
 * A250608  Number of (n+1) X (5+1) 0..1 arrays with nondecreasing
 *          x(i,j)-x(i,j-1) in the i direction and nondecreasing
 *          x(i,j)-x(i-1,j) in the j direction.
 * https://oeis.org/A250608
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 144*2^(n-1) + 42*n^2 + 69*n - 8
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 25, u64 n <= 57, u128 n <= 121,
 *            big any n
 * Helpers:   mul_small, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mul_small.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Ensure the input is at least 1 (the sequence offset) */
    if (n < 1)
        return 0;

    seq_t nseq = (seq_t)n;

    /* 144 * 2^(n-1) */
    seq_t power = pow_seq((seq_t)2, (int)(n - 1));
    power = mul_small(power, 144);

    /* 42*n^2 + 69*n */
    seq_t poly = (seq_t)42 * (nseq * nseq) + (seq_t)69 * nseq;

    /* Combine and subtract the constant offset 8 */
    return power + poly - (seq_t)8;
}

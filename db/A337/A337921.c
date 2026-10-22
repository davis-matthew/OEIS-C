/*
 * A337921  a(n) is the sum of (3^n mod 2^k) for k such that 2^k < 3^n.
 * https://oeis.org/A337921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   bitlen_seq_t, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = Σ (3^n mod 2^k) for all k with 2^k < 3^n
 * (OEIS sequence A337921)
 *
 * We calculate 3^n exactly (pow3), determine the maximum admissible
 * exponent K = floor(log2(3^n)), and then use the binary
 * representation of 3^n to obtain the sum from the lower K bits.
 * For each bit b_i (0 ≤ i < K) that is 1, it contributes
 * (K-i)·2^i to the total sum.  The loop runs in O(K) time
 * (K ≈ 1.585·n), which is easily fast enough for n up to 10 000.
 */

#include "../utils/pow3.h"
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    /* 3^n */
    seq_t x = pow3(n);

    /* maximum k with 2^k < 3^n is floor(log2(3^n)) = bitlen-1 */
    idx_t K = bitlen_seq_t(x) - 1;

    seq_t sum = 0;
    for (idx_t i = 0; i < K; ++i) {
        /* test bit i of x */
        if (((x >> i) & 1) != 0) {
            /* contribution of this bit: (K-i) * 2^i */
            sum += ((seq_t)(K - i)) << i;
        }
    }
    return sum;
}

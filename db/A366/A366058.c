/*
 * A366058  Number of n-step self-avoiding walks on a 3D cubic lattice where
 *          no step is to a lattice point closer to the origin than the
 *          current point.
 * https://oeis.org/A366058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8 * 3^n - 12 * 2^n + 6 (n > 0), a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t pow3_n = pow_seq((seq_t)3, n);
    seq_t pow2_n = pow_seq((seq_t)2, n);

    seq_t res = (seq_t)8 * pow3_n;
    res = res - (seq_t)12 * pow2_n + (seq_t)6;
    return res;
}

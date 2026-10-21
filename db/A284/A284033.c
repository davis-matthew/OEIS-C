/*
 * A284033  Poly-Bernoulli numbers B_n^(k) with k = -9.
 * https://oeis.org/A284033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 362880*10^n - 1451520*9^n + 2328480*8^n - 1905120*7^n +
 *            834120*6^n - 186480*5^n + 18150*4^n - 510*3^n + 2^n
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 14, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow_const(unsigned int base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    while (exp)
    {
        if (exp & 1)
            result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t p10 = pow_const(10, n);
    seq_t p9  = pow_const(9, n);
    seq_t p8  = pow_const(8, n);
    seq_t p7  = pow_const(7, n);
    seq_t p6  = pow_const(6, n);
    seq_t p5  = pow_const(5, n);
    seq_t p4  = pow_const(4, n);
    seq_t p3  = pow_const(3, n);
    seq_t p2  = pow_const(2, n);

    seq_t result = (seq_t)362880 * p10;
    result -= (seq_t)1451520 * p9;
    result += (seq_t)2328480 * p8;
    result -= (seq_t)1905120 * p7;
    result += (seq_t)834120 * p6;
    result -= (seq_t)186480 * p5;
    result += (seq_t)18150 * p4;
    result -= (seq_t)510 * p3;
    result += p2;
    return result;
}

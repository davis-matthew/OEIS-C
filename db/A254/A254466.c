/*
 * A254466  a(n) = 56*2^n + 20*4^n + 35*3^n + 4*6^n + 10*5^n + 7^n + 84.
 * https://oeis.org/A254466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 56*2^n + 20*4^n + 35*3^n + 4*6^n + 10*5^n + 7^n + 84
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t pow2 = pow_seq( (seq_t)2, n );
    seq_t pow3 = pow_seq( (seq_t)3, n );
    seq_t pow4 = pow_seq( (seq_t)4, n );
    seq_t pow5 = pow_seq( (seq_t)5, n );
    seq_t pow6 = pow_seq( (seq_t)6, n );
    seq_t pow7 = pow_seq( (seq_t)7, n );

    seq_t val = 0;
    val += (seq_t)56 * pow2;
    val += (seq_t)20 * pow4;
    val += (seq_t)35 * pow3;
    val += (seq_t) 4 * pow6;
    val += (seq_t)10 * pow5;
    val += pow7;
    val += (seq_t)84;
    return val;
}

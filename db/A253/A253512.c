/*
 * A253512  a(n) = (2^n - 1) * (3^(n+2) - 1) / 2.
 * https://oeis.org/A253512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^n - 1) * (3^(n+2) - 1) / 2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t seq_pow_uint(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t cur    = base;
    while (exp > 0) {
        if (exp & (idx_t)1) {
            result *= cur;
        }
        if (exp > 1) {
            cur *= cur;
        }
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t pow2 = seq_pow_uint((seq_t)2, n);
    seq_t pow3 = seq_pow_uint((seq_t)3, n + 2);
    seq_t t1   = pow2 - (seq_t)1;
    seq_t t2   = pow3 - (seq_t)1;
    return (t1 * t2) / (seq_t)2;
}

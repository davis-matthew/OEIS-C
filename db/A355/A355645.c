/*
 * A355645  The number of regions in the G-Shi arrangement when G is the cycle
 *          graph C_n.
 * https://oeis.org/A355645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - 2^n - n (for n >= 2), a(1)=1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    seq_t three_pow = pow_seq((seq_t)3, n);
    seq_t two_pow  = pow_seq((seq_t)2, n);
    seq_t n_seq    = (seq_t)n;
    return three_pow - two_pow - n_seq;
}

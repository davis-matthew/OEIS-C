/*
 * A269761  Number of n X 2 0..5 arrays with some element plus some
 *          horizontally or vertically adjacent neighbor totalling five
 *          exactly once.
 * https://oeis.org/A269761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=6; a(2)=480; a(n) = 80 * 3^(n-2) * 7^(n-3) * (31*n-20) for
 *            n >= 3 (OEIS A269761)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp != (idx_t)0) {
        if (exp & (idx_t)1)
            result = result * base;
        base = base * base;
        exp >>= (idx_t)1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)6;
    if (n == (idx_t)2)
        return (seq_t)480;

    seq_t p3 = pow_seq((seq_t)3, n - (idx_t)2);   /* 3^(n-2) */
    seq_t p7 = pow_seq((seq_t)7, n - (idx_t)3);   /* 7^(n-3) */

    seq_t result = p3 * p7;
    result *= (seq_t)80;
    seq_t factor = (seq_t)(31 * n - (idx_t)20);
    result *= factor;
    return result;
}

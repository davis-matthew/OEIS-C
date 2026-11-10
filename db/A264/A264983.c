/*
 * A264983  Odd bisection of A263273.
 * https://oeis.org/A264983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A263273(2n + 1), where A263273(m) = A030102(A038502(m)) *
 *            A038500(m) (reverse the base-3 digits of m with trailing zeros
 *            stripped, then multiply by the 3-power factor)
 * Range:     u8 n <= 121, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9294 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A263273_a(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t temp = n;
    seq_t p3 = (seq_t)1;
    while ((temp % (idx_t)3) == 0) {
        temp /= (idx_t)3;
        p3 *= (seq_t)3;
    }
    seq_t rev = (seq_t)0;
    idx_t m = temp;
    while (m > 0) {
        rev = rev * (seq_t)3 + (seq_t)(m % (idx_t)3);
        m /= (idx_t)3;
    }
    return rev * p3;
}

seq_t a(idx_t n)
{
    return A263273_a((idx_t)2 * n + (idx_t)1);
}

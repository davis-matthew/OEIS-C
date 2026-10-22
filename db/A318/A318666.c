/*
 * A318666  a(n) = 2^{the 3-adic valuation of n}.
 * https://oeis.org/A318666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 2^{v_3(n)} (the 3‑adic valuation of n)
 * Range:     u8 n <= 6560, u16 n <= 9653, u32 n <= 9653, u64 n <= 9653,
 *            u128 n <= 9653, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t t = n;
    idx_t e = 0;
    while ((t % 3) == 0) {
        t /= 3;
        ++e;
    }
    seq_t result = 1;
    for (idx_t i = 0; i < e; ++i) {
        result = result * 2;
    }
    return result;
}

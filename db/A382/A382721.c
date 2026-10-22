/*
 * A382721  Number of entries in the n-th row of Pascal's triangle not
 *          divisible by 11.
 * https://oeis.org/A382721
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏ (digit_i(n in base 11) + 1)
 * Range:     u8 n <= 328, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of entries in the n‑th row of Pascal's triangle not divisible by 11 */
seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;
    idx_t m = n;
    while (m != 0) {
        seq_t d = (seq_t)(m % (idx_t)11);
        res *= (d + (seq_t)1);
        m /= (idx_t)11;
    }
    return res;
}

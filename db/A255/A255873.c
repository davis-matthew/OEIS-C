/*
 * A255873  The first nonzero digit of n/7.
 * https://oeis.org/A255873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = first non‑zero digit of n/7 (in base 10)
 * Range:     u8 n <= 1524, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Integer part of n/7 */
    seq_t q = (seq_t)n / 7;
    if (q != 0) {
        /* Return the most significant digit of q */
        while (q >= 10) q /= 10;
        return q;
    }
    /* n < 7: fractional part only, first non‑zero digit after decimal point */
    seq_t rem = (seq_t)n;
    seq_t digit;
    do {
        digit = (rem * 10) / 7;
        if (digit != 0) return digit;
        rem = (rem * 10) % 7;
    } while (1);
}

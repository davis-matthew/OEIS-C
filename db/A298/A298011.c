/*
 * A298011  If n = Sum_{i=1..h} 2^b_i with 0 <= b_1 < ... < b_h, then a(n) =
 *          Sum_{i=1..h} i * 2^b_i.
 * https://oeis.org/A298011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = ∑_{i=1..h} i * 2^b_i, where n = ∑_{i=1}^h 2^{b_i} (binary
 *            representation)
 * Range:     u8 n <= 60, u16 n <= 6142, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t result = 0;
    seq_t power   = 1;      /* 2^0 */
    idx_t rank    = 0;
    idx_t t       = n;
    while (1) {
        if (t & 1) {
            rank++;
            result += (seq_t)rank * power;
        }
        t >>= 1;
        if (!t)
            break;
        power <<= 1;
    }
    return result;
}

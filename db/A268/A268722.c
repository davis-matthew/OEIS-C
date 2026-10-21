/*
 * A268722  a(n) = A003188(3*A006068(n)), where A003188 is binary Gray code
 *          and A006068 is its inverse.
 * https://oeis.org/A268722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003188(3*A006068(n)) A003188(m) = m ^ (m >> 1) – binary
 *            Gray code A006068(n) = inverse of Gray code – computes binary
 *            from Gray
 * Range:     u8 n <= 63, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Gray code of the index (used as inverse input) */
    seq_t g = (seq_t)n;

    /* Inverse Gray: recover the binary number of which g is the Gray code */
    seq_t bin = g;
    seq_t s   = g >> 1;
    while (s) {
        bin ^= s;
        s >>= 1;
    }

    /* Multiply by 3 and apply Gray code again */
    seq_t m = bin * (seq_t)3;
    return m ^ (m >> 1);
}

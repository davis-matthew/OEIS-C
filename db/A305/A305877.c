/*
 * A305877  Numbers in base 3 reversed.
 * https://oeis.org/A305877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = reverse of the base‑3 representation of n, interpreted in
 *            base‑10
 * Range:     u8 n <= 27, u16 n <= 243, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t x = n;
    seq_t rev = 0;
    while (x != 0) {
        seq_t d = (seq_t)(x % 3);
        x /= 3;
        rev = rev * 10 + d;
    }
    return rev;
}

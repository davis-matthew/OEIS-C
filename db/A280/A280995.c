/*
 * A280995  a(n) is the number produced when n is converted to binary
 *          reflected Gray code, the binary digits are reversed and the code
 *          is converted back to decimal.
 * https://oeis.org/A280995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = reverse_bits( n ^ (n >> 1), floor_log2(n)+1 )
 * Range:     u8 n <= 256, u16 n <= 9611, u32 n <= 9611, u64 n <= 9611,
 *            u128 n <= 9611, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = 0;
    idx_t t = n;
    while (t > 0) {
        ++k;
        t >>= 1;
    }
    if (k == 0)   /* handle n = 0 */
        k = 1;

    idx_t g = n ^ (n >> 1);
    idx_t rev = 0;
    for (idx_t i = 0; i < k; ++i) {
        rev |= ((g >> i) & 1ULL) << (k - 1 - i);
    }
    return (seq_t)rev;
}

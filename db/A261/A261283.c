/*
 * A261283  a(n) = bitwise XOR of all the bit numbers for the bits that are
 *          set in n, using number 1 for the LSB.
 * https://oeis.org/A261283
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = XOR_{i >= 0} ( (n >> i) & 1 ? (i+1) : 0 )
 * Range:     u8 n <= 8190, u16 n <= 8190, u32 n <= 8190, u64 n <= 8190,
 *            u128 n <= 8190, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t bit = 0;
    while (n) {
        if (n & 1U) {
            result ^= (seq_t)(bit + 1);
        }
        n >>= 1;
        ++bit;
    }
    return result;
}

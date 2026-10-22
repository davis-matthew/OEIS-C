/*
 * A326729  a(0) = 0; for n >= 1, a(n) is the result of inverting s-th bit
 *          (from right) in n, where s is the number of ones in the binary
 *          representation of n.
 * https://oeis.org/A326729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(0) = 0; for n ≥ 1, a(n) = n XOR 2^(popcount(n)-1)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* count ones in binary representation of n */
    idx_t m = n;
    idx_t pc = 0;
    while (m) {
        pc += m & 1;
        m >>= 1;
    }

    /* the s‑th bit to invert is the (pc‑1)‑th (0‑based) bit from the LSB */
    seq_t mask = ((seq_t)1) << (pc - 1);
    return ((seq_t)n) ^ mask;
}

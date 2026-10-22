/*
 * A337623  a(n) is the least positive multiple of 2*n-1 containing only the
 *          digits 0 and 1 in base n.
 * https://oeis.org/A337623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = read_base_n(reverse(write_base_2(2*n-1)))
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 2*n - 1 */
    idx_t m = (idx_t)2 * n - 1;

    /* find number of bits of m */
    idx_t bitlen = 0;
    idx_t tmp = m;
    while (tmp) {
        tmp >>= 1;
        ++bitlen;
    }
    if (bitlen == 0)
        bitlen = 1;                      /* safety for n=1 */

    /* interpret the reversed binary bits as a base‑n number */
    seq_t res = 0;
    for (idx_t i = 0; i < bitlen; ++i) { /* bits from LSB to MSB */
        int bit = (m >> i) & 1;
        res = res * (seq_t)n + (seq_t)bit;
    }
    return res;
}

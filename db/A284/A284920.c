/*
 * A284920  Numbers with digits 2 and 4 only.
 * https://oeis.org/A284920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2 * A007931(n)
 * Range:     u8 n <= 10, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Zero‑based index */
    idx_t n0 = n - 1;
    idx_t cum = 0;
    idx_t power = 2;   /* 2^1 for length 1 numbers */
    idx_t L = 1;       /* current length */

    while (cum + power <= n0)
    {
        cum += power;
        ++L;
        power <<= 1;   /* power = 2^L */
    }

    /* Offset within numbers of length L (0‑based) */
    idx_t offset = n0 - cum;

    seq_t res = 0;
    for (idx_t i = 0; i < L; ++i)
    {
        seq_t digit = ((offset >> (L - 1 - i)) & 1) ? 4 : 2;
        res = res * 10 + digit;
    }
    return res;
}

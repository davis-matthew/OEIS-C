/*
 * A329530  a(n) = n * (7*binomial(n, 2) + 1).
 * https://oeis.org/A329530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * (7*binomial(n, 2) + 1)
 * Range:     u8 n <= 4, u16 n <= 26, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t t;
    if (n < 2)
        t = 0;
    else
        t = nseq * (nseq - 1) / 2;
    seq_t res = nseq * ((seq_t)7 * t + 1);
    return res;
}

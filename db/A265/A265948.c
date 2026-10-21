/*
 * A265948  Numbers whose name in German contains the letter Ö (O with
 *          Umlaut).
 * https://oeis.org/A265948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = 12 + 100*(n-1)
 * Range:     u8 n <= 3, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nseq = (seq_t)n;
    seq_t term  = (seq_t)12 + (seq_t)100 * (nseq - 1);
    return term;
}

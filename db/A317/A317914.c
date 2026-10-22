/*
 * A317914  a(n) = 142099325379199423 + (n-1)*3691994023167450.
 * https://oeis.org/A317914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 142099325379199423 + (n-1)*3691994023167450
 * Range:     u8 none, u16 none, u32 none, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)142099325379199423ULL + ((seq_t)n - 1) * (seq_t)3691994023167450ULL;
}

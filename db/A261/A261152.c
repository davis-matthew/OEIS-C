/*
 * A261152  a(n) = 161004359399459161 + (n-1)*10644900609172830.
 * https://oeis.org/A261152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 161004359399459161 + (n-1)*10644900609172830
 * Range:     u8 none, u16 none, u32 none, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)161004359399459161ULL;
    seq_t step = (seq_t)10644900609172830ULL;
    return base + ((seq_t)(n - 1)) * step;
}

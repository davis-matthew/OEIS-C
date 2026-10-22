/*
 * A344317  a(n) = n*a(n-1) + n^(1+n mod 2), a(0) = 1.
 * https://oeis.org/A344317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = n*a(n-1) + (n even ? n : 1) for n > 0 (OEIS
 *            A344317)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t value = 1;                 /* a(0) */
    for (idx_t i = 1; i <= n; ++i) {
        value = ((seq_t)i) * value   /* n*a(n-1) */
              + ((i & 1) ? 1 : (seq_t)i); /* +n if n even, +1 if n odd */
    }
    return value;
}

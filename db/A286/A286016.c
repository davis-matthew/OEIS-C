/*
 * A286016  Signed continued fraction expansion with all signs negative of
 *          tanh(1).
 * https://oeis.org/A286016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   a(1)=1; for m≥0, if n=2m²+3m+2 then a(n)=5+4m, otherwise a(n)=2
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    idx_t m    = 0;
    idx_t index = 2 * m * m + 3 * m + 2;  /* 2m²+3m+2 */

    while (index < n) {
        ++m;
        index = 2 * m * m + 3 * m + 2;
    }

    if (n == index)
        return (seq_t)(5 + 4 * m);

    return (seq_t)2;
}

/*
 * A385938  a(n) = 2*n/3 if n == 0 (mod 3), (2*n+1)/3 if n == 1 (mod 3),
 *          (7*n+1)/3 if n == 2 (mod 3).
 * https://oeis.org/A385938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n/3 if n == 0 (mod 3), (2*n+1)/3 if n == 1 (mod 3),
 *            (7*n+1)/3 if n == 2 (mod 3)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 3;
    seq_t k = (seq_t)n;
    switch (r)
    {
        case 0: return k * 2 / 3;
        case 1: return (k * 2 + 1) / 3;
        case 2: return (k * 7 + 1) / 3;
        default: return 0; /* unreachable */
    }
}

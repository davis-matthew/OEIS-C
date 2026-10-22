/*
 * A332891  Sum of the widths of all r X s rectangles such that r < s, r + s =
 *          2n and (s - r) | (s * r).
 * https://oeis.org/A332891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n-1} i * (1 - ceil(i*(2*n-i)/(2*n-2*i)) +
 *            floor(i*(2*n-i)/(2*n-2*i)))
 * Range:     u8 n <= 16, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    seq_t sum = (seq_t)0;
    seq_t ns = (seq_t)n;
    seq_t ns2 = (seq_t)2 * ns;                      // 2*n
    for (idx_t ii = 1; ii < n; ++ii)
    {
        seq_t i = (seq_t)ii;
        seq_t denom = (seq_t)2 * (ns - i);          // 2*(n-i)
        seq_t num = i * (ns2 - i);                 // i*(2*n-i)
        if (num % denom == 0)
            sum += i;
    }
    return sum;
}

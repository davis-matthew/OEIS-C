/*
 * A352745  a(n) is the number of Lyndon factors of the Fibonacci string of
 *          length n.
 * https://oeis.org/A352745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 1 for n <= 2; a(n) = n-2 if n even and n >= 4; a(n) =
 *            (n+1)/2 if n odd
 * Range:     u8 n <= 46, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2) return (seq_t)1;
    if ((n & 1ULL) == 0) return (seq_t)(n - 2);
    return (seq_t)((n + 1) >> 1);
}

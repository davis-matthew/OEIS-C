/*
 * A346663  The number of nonreal roots of Sum_{k=0..n} prime(k+1)*x^k.
 * https://oeis.org/A346663
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-1 for odd n;
   a(n) = n for even n < 2436;
   a(n) = n-2 for even n >= 2436
   (this matches the known values of A346663). */
seq_t a(idx_t n)
{
    if (n & 1ULL)                 /* odd */
        return (seq_t)n - 1ULL;
    else if (n < 2436ULL)         /* even, small n */
        return (seq_t)n;
    else                          /* even, n >= 2436 */
        return (seq_t)n - 2ULL;
}

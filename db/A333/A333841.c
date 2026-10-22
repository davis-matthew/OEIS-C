/*
 * A333841  Integers n such that n! = x^2 + y^3 + z^4 where x, y and z are
 *          nonnegative integers, is soluble.
 * https://oeis.org/A333841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-1 for n ≤ 5; a(n) = 6 for n = 6; a(n) = n+1 for n ≥ 7
 * Range:     u8 n <= 23, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (n is the sequence index, 1‑based. a(1)=0, a(2)=1, a(3)=2, a(4)=3, a(5)=4, a(6)=6, a(n)=n+1 for n≥7) */
seq_t a(idx_t n)
{
    if (n <= 5)
        return (seq_t)(n - 1);
    if (n == 6)
        return (seq_t)6;
    return (seq_t)(n + 1);
}

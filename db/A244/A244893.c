/*
 * A244893  a(n) = a(n-a(n-1)) with initial values 2,3,2.
 * https://oeis.org/A244893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2 if n == 1 or n % 3 == 0, otherwise 3.
 */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n % 3 == 0) return (seq_t)2;
    return (seq_t)3;
}

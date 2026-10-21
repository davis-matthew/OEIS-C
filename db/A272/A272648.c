/*
 * A272648  a(n) = A002119(n) mod 7.
 * https://oeis.org/A272648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A002119(n) mod 7 Equivalent short form: let m = n mod 7
 *            if m==2 or m==4: a(n)=0 else if floor(n/7) is even: a(n)=1 else:
 *            a(n)=6
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n % 7;
    if (m == 2 || m == 4)
        return (seq_t)0;
    if ((n / 7) & 1u)          /* odd quotient => 6 */
        return (seq_t)6;
    return (seq_t)1;           /* even quotient => 1 */
}

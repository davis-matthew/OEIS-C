/*
 * A269222  Period 4: repeat [1,9,8,9].
 * https://oeis.org/A269222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-4) for n >=5; a(1)=1, a(2)=9, a(3)=8, a(4)=9
 *            (periodic with [1,9,8,9])
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n % 4)
    {
        case 1: return (seq_t)1;
        case 2: return (seq_t)9;
        case 3: return (seq_t)8;
        default: return (seq_t)9; /* n % 4 == 0 */
    }
}

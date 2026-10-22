/*
 * A387540  Number of simply laced Dynkin diagrams with n nodes.
 * https://oeis.org/A387540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n = 0,1,2,3; a(4)=2; a(5)=2; a(6)=3; a(7)=3;
 *            a(8)=3; a(n)=2 for n > 8
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1 || n == 2 || n == 3)
        return (seq_t)1;
    if (n == 4 || n == 5)
        return (seq_t)2;
    if (n == 6 || n == 7 || n == 8)
        return (seq_t)3;
    return (seq_t)2;   /* for n > 8 */
}

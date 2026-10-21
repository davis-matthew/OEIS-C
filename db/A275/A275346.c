/*
 * A275346  In Go, minimum total number of liberties player 1 (black) can have
 *          on a standard 19 X 19 board after n moves when no player passes a
 *          move, with no repeating game positions allowed.
 * https://oeis.org/A275346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2, a(2)=1, a(3)=2, a(4)=1, a(5)=1, and for n>=6, a(n)=
 *            (n%2==0?0:1)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)2;
    if (n == (idx_t)2) return (seq_t)1;
    if (n == (idx_t)3) return (seq_t)2;
    if (n == (idx_t)4) return (seq_t)1;
    if (n == (idx_t)5) return (seq_t)1;
    return (n & (idx_t)1) ? (seq_t)1 : (seq_t)0;
}

/*
 * A267043  Middle column of the "Rule 91" elementary cellular automaton
 *          starting with a single ON (black) cell.
 * https://oeis.org/A267043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(1)=a(2)=a(3)=0; a(4)=1; for n>4, a(n)=a(n-2). Thus
 *            a(n)=1 when n%2==0 and n>=4, otherwise a(n)=0
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)               return 1;
    if (n < 4)                return 0;
    /* n >= 4 */
    return (n & 1U) == 0 ? (seq_t)1 : (seq_t)0;
}

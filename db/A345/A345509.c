/*
 * A345509  Numbers that are the sum of ten squares in two or more ways.
 * https://oeis.org/A345509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 224, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = n + 31 for n > 7,
 *  a(1)=25, a(2)=28, a(3)=31, a(4)=33, a(5)=34, a(6)=36, a(7)=37
 * (Indexes start at 1)
 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* defensive, not required by spec */
    if (n <= 7) {
        static const seq_t tbl[7] = {
            (seq_t)25, (seq_t)28, (seq_t)31, (seq_t)33, (seq_t)34,
            (seq_t)36, (seq_t)37
        };
        return tbl[n-1];
    }
    return (seq_t)n + (seq_t)31;
}

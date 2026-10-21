/*
 * A291584  The arithmetic function uhat(n,8,8).
 * https://oeis.org/A291584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-8) for n > 8 with initial terms * a(1)=1, a(2)=0,
 *            a(3)=1, a(4)=-2, a(5)=1, a(6)=0, a(7)=1, a(8)=-6
 * Range:     i8 n <= 70, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t tbl[8] = { (seq_t)1, (seq_t)0, (seq_t)1,
                                 (seq_t)-2, (seq_t)1, (seq_t)0,
                                 (seq_t)1, (seq_t)-6 };
    return tbl[(n - 1) % 8];
}

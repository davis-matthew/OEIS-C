/*
 * A397974  Lower (1,2/3) midsequence of triangular numbers (A000217) and
 *          squares (A000290); see Comments.
 * https://oeis.org/A397974
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*(n+1)/2 + (2/3)*n^2) Recurrence: a(0)=0, a(1)=1,
 *            a(2)=5, a(3)=12, a(4)=20;
 *            a(n)=2*a(n-1)-a(n-2)+a(n-3)-2*a(n-4)+a(n-5) for n≥5 The direct
 *            formula is used for O(1) computation
 * Range:     u8 n <= 14, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                     /* cast index to seq_t */
    seq_t s  = nn * (nn + 1) / 2;            /* triangular number t(n) */
    seq_t val = (s * 3 + 2 * nn * nn) / 3;   /* floor((3*s + 2*n^2)/3) */
    return val;
}

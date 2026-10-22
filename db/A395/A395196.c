/*
 * A395196  Minimal value of Sum_{i=1..n} p(i)*p(i+1), where p(n+1)=p(1), as p
 *          ranges over all permutations of odd numbers {1,3,...,2*n-1}.
 * https://oeis.org/A395196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (2*n^3 + 7*n - 9 - 3*(-1)^n)/3
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n in seq_t, enforce unsigned subtraction is safe */
    seq_t nn = (seq_t)n;
    seq_t nn3 = nn * nn * nn;           /* n^3 */
    seq_t sum;

    if ((n & 1) == 0) /* n even */
        sum = 2 * nn3 + 7 * nn - 12;    /* 2*n^3 + 7*n -12 */
    else               /* n odd  */
        sum = 2 * nn3 + 7 * nn - 6;     /* 2*n^3 + 7*n -6 */

    return sum / 3;
}

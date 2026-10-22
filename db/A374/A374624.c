/*
 * A374624  a(n) is the number of irreducible finite Coxeter groups in n
 *          dimensions, or -1 if there are an infinite number.
 * https://oeis.org/A374624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) is the number of irreducible finite Coxeter groups * in n
 *            dimensions, or -1 if there are infinitely many such groups
 * Range:     i8 n <= 86, i16 n <= 86, i32 n <= 86, i64 n <= 86, i128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)      return 0;          /* undefined for n<1 */
    if (n == 1)     return 1;          /* A1            */
    if (n == 2)     return -1;         /* infinitely many I2(m)   */

    seq_t count = 1;                   /* A(n)          */
    if (n >= 2)    count += 1;         /* B(n) (≡C(n))  */
    if (n >= 4)    count += 1;         /* D(n)          */

    /* Special additional types */
    if (n == 3)    count += 1;         /* H3            */
    if (n == 4)    count += 2;         /* F4 and H4     */
    if (n == 6)    count += 1;         /* E6            */
    if (n == 7)    count += 1;         /* E7            */
    if (n == 8)    count += 1;         /* E8            */

    return count;
}

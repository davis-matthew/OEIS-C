/*
 * A246453  Lucas numbers (A000204) of the form n^2 + 2.
 * https://oeis.org/A246453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=3, a(2)=11, a(3)=18, a(4)=123 Recurrence: a(n) = 7*a(n-1) -
 *            a(n-2) for n>4
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial values directly */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)11;
    if (n == 3) return (seq_t)18;
    if (n == 4) return (seq_t)123;

    /* Iterative computation for n >= 5 */
    seq_t a_nm2 = (seq_t)18;   /* a(n-2)  ← 18  */
    seq_t a_nm1 = (seq_t)123;  /* a(n-1)  ← 123 */
    seq_t a_n;                 /* current value */

    for (idx_t i = 5; i <= n; ++i) {
        a_n = (seq_t)7 * a_nm1 - a_nm2;  /* a(n) = 7*a(n-1) - a(n-2) */
        a_nm2 = a_nm1;                   /* shift previous values */
        a_nm1 = a_n;
    }
    return a_nm1;
}

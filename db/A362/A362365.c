/*
 * A362365  The sum of the coefficients of x^k in the expansion of (x + x^2 +
 *          x^3 + x^4 + x^5 + x^6)^n with k divisible by 4.
 * https://oeis.org/A362365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-1) + 10*a(n-2) + 12*a(n-3);
   initial values: a(1)=1, a(2)=9, a(3)=55.
   This implements OEIS A362365. */
seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)9;
    if (n == (idx_t)3) return (seq_t)55;

    seq_t a_nm3 = (seq_t)1;   /* a(n-3)  */
    seq_t a_nm2 = (seq_t)9;   /* a(n-2)  */
    seq_t a_nm1 = (seq_t)55;  /* a(n-1)  */
    seq_t a_n   = 0;          /* a(n)    */

    for (idx_t i = (idx_t)4; i <= n; ++i) {
        a_n = (seq_t)4 * a_nm1 + (seq_t)10 * a_nm2 + (seq_t)12 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}

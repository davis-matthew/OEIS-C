/*
 * A319170  Triangular numbers of the form 2..21..1; n_times 2 followed with
 *          n_times 1; n >= 1.
 * https://oeis.org/A319170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1/9 * (2*10^n + 1) * (10^n - 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the linear recurrence:        */
/*   a(1) = 21,   a(2) = 2211,   a(3) = 222111          */
/*   a(n) = 111*a(n-1) - 1110*a(n-2) + 1000*a(n-3)  (n > 3) */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)21;
    if (n == 2) return (seq_t)2211;
    if (n == 3) return (seq_t)222111;

    seq_t a_nm3 = (seq_t)21;      /* a(n-3)  */
    seq_t a_nm2 = (seq_t)2211;    /* a(n-2)  */
    seq_t a_nm1 = (seq_t)222111;  /* a(n-1)  */
    seq_t an;                    /* a(n)    */

    for (idx_t i = 4; i <= n; ++i) {
        an = (seq_t)111 * a_nm1 - (seq_t)1110 * a_nm2 + (seq_t)1000 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = an;
    }
    return an;
}

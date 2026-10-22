/*
 * A331321  a(n) = [x^n] ((x^2 - 1)*(x^2 + x - 1))/(x^2 + 2*x - 1)^2.
 * https://oeis.org/A331321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 4*a(n-3) - a(n-4) (n > 4) Initial
 *            values a(0)=1, a(1)=3, a(2)=8, a(3)=23, a(4)=64
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 47, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Fast return for first five indices */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)23;
    if (n == 4) return (seq_t)64;

    /* Variables holding the last four computed values:
       a_nm4 = a(n-4), a_nm3 = a(n-3), a_nm2 = a(n-2), a_nm1 = a(n-1),
       curr   = a(n-1) before the iteration step
    */
    seq_t a_nm4 = (seq_t)1;   /* a0 */
    seq_t a_nm3 = (seq_t)3;   /* a1 */
    seq_t a_nm2 = (seq_t)8;   /* a2 */
    seq_t a_nm1 = (seq_t)23;  /* a3 */
    seq_t curr  = (seq_t)64;  /* a4 */

    /* Iterate from n = 5 up to the desired index */
    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = (seq_t)4 * curr
                   - (seq_t)2 * a_nm1
                   - (seq_t)4 * a_nm2
                   - a_nm3;

        /* shift the windows for the next iteration */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = curr;
        curr = next;
    }

    return curr; /* curr now holds a(n) */
}

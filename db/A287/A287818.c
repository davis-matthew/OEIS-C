/*
 * A287818  Number of nonary sequences of length n such that no two
 *          consecutive terms have distance 3.
 * https://oeis.org/A287818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A287818
   Formula: a(n) = 9*a(n-1) - 10*a(n-2)  (n > 2), with a(0)=1, a(1)=9, a(2)=69 */

seq_t a(idx_t n)
{
    /* handle the three base cases first */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)9;
    if (n == 2u) return (seq_t)69;

    /* iterative computation for n >= 3 */
    seq_t a_nm2 = (seq_t)9;   /* a(n-2) : a(1)   */
    seq_t a_nm1 = (seq_t)69;  /* a(n-1) : a(2)   */
    seq_t a_n   = 0;          /* current a(n)    */

    for (idx_t i = 3; i <= n; ++i) {
        a_n = (seq_t)9 * a_nm1 - (seq_t)10 * a_nm2;
        /* shift: next iteration will use these as a_nm2 and a_nm1 */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}

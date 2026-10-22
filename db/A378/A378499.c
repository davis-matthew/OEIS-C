/*
 * A378499  a(n) is the number whose base-4 digits are 1 followed by the first
 *          n-1 terms of the periodic sequence with initial period 0,3,2.
 * https://oeis.org/A378499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A378499: a(n) = floor((11/36)·4^n)
   Recurrence: a(n) = 4*a(n-1) + a(n-3) - 4*a(n-4)
   Initial values: a(1)=1, a(2)=4, a(3)=19, a(4)=78
   This function works for any n ≥ 1, in any call order. */
seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)19;
    if (n == 4) return (seq_t)78;

    /* Variables hold a(k-4), a(k-3), a(k-2), a(k-1) respectively. */
    seq_t a_nm4 = (seq_t)1;   /* a(1)   */
    seq_t a_nm3 = (seq_t)4;   /* a(2)   */
    seq_t a_nm2 = (seq_t)19;  /* a(3)   */
    seq_t a_nm1 = (seq_t)78;  /* a(4)   */

    for (idx_t k = 5; k <= n; ++k) {
        /* Compute a(k) using the recurrence. */
        seq_t an = 4 * a_nm1 + a_nm3 - 4 * a_nm4;

        /* Shift variables for the next iteration. */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = an;
    }

    return a_nm1;
}
